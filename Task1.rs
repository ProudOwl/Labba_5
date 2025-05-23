use std::collections::HashMap;
use std::io;
use std::fmt;

struct Cell {
    address: String,
    product: String,
    quantity: i32,
}

struct Warehouse {
    zones: i32,
    shelves_per_zone: i32,
    sections_per_shelf: i32,
    shelves_per_section: i32,
    total_capacity: i32,
    used_capacity: i32,
    cells: HashMap<String, Cell>,
}

impl Warehouse {
    fn new(z: i32, spz: i32, sec: i32, spl: i32, cap: i32) -> Self {
        Warehouse {
            zones: z,
            shelves_per_zone: spz,
            sections_per_shelf: sec,
            shelves_per_section: spl,
            total_capacity: cap,
            used_capacity: 0,
            cells: HashMap::new(),
        }
    }

    fn is_valid_address(&self, address: &str) -> bool {
        if address.len() != 5 {
            return false;
        }
        
        let zone_char = address.chars().nth(0).unwrap();
        let zone = zone_char as i32 - 'A' as i32;
        if zone < 0 || zone >= self.zones {
            return false;
        }
        
        let shelf_str = &address[1..3];
        let shelf = match shelf_str.parse::<i32>() {
            Ok(num) => num,
            Err(_) => return false,
        };
        if shelf < 1 || shelf > self.shelves_per_zone {
            return false;
        }
        
        let section_char = address.chars().nth(3).unwrap();
        let section = section_char as i32 - '0' as i32;
        if section < 1 || section > self.sections_per_shelf {
            return false;
        }
        
        let shelf_num_char = address.chars().nth(4).unwrap();
        let shelf_num = shelf_num_char as i32 - '0' as i32;
        if shelf_num < 1 || shelf_num > self.shelves_per_section {
            return false;
        }
        
        true
    }

    fn add_product(&mut self, product: &str, quantity: i32, address: &str) {
        if !self.is_valid_address(address) {
            println!("Неверный адрес: {}", address);
            return;
        }

        if quantity > 10 {
            println!("Нельзя добавить более 10 единиц в ячейку");
            return;
        }

        if let Some(cell) = self.cells.get_mut(address) {
            if cell.product != product {
                println!("Ячейка {} уже содержит {}", address, cell.product);
                return;
            }
            if cell.quantity + quantity > 10 {
                println!("Ячейка {} не может содержать более 10 единиц (сейчас: {})", 
                         address, cell.quantity);
                return;
            }
            cell.quantity += quantity;
        } else {
            self.cells.insert(address.to_string(), Cell {
                address: address.to_string(),
                product: product.to_string(),
                quantity,
            });
        }
        self.used_capacity += quantity;
        println!("Добавлено {} единиц {} в {}", quantity, product, address);
    }

    fn remove_product(&mut self, product: &str, quantity: i32, address: &str) {
        if !self.is_valid_address(address) {
            println!("Неверный адрес: {}", address);
            return;
        }

        if !self.cells.contains_key(address) {
            println!("Ячейка {} пуста", address);
            return;
        }

        let cell = self.cells.get_mut(address).unwrap();
        if cell.product != product {
            println!("Ячейка {} содержит {}, а не {}", address, cell.product, product);
            return;
        }

        if cell.quantity < quantity {
            println!("Недостаточно {} в ячейке {} (доступно: {})", 
                     product, address, cell.quantity);
            return;
        }

        cell.quantity -= quantity;
        self.used_capacity -= quantity;
        println!("Удалено {} единиц {} из {}", quantity, product, address);

        if cell.quantity == 0 {
            self.cells.remove(address);
        }
    }

    fn print_info(&self) {
        let total_percent = (self.used_capacity as f64 / self.total_capacity as f64) * 100.0;
        println!("Склад заполнен на {:.1}%", total_percent);

        let mut zone_capacity: HashMap<char, i32> = HashMap::new();
        let mut zone_used: HashMap<char, i32> = HashMap::new();
        
        for zone in 'A'..('A' as u8 + self.zones as u8) as char {
            let cap = self.shelves_per_zone * self.sections_per_shelf * self.shelves_per_section * 10;
            zone_capacity.insert(zone, cap);
            zone_used.insert(zone, 0);
        }

        for (_, cell) in &self.cells {
            let zone = cell.address.chars().next().unwrap();
            *zone_used.get_mut(&zone).unwrap() += cell.quantity;
        }

        for zone in 'A'..('A' as u8 + self.zones as u8) as char {
            let used = zone_used[&zone];
            let cap = zone_capacity[&zone];
            let zone_percent = (used as f64 / cap as f64) * 100.0;
            println!("Зона {} заполнена на {:.1}%", zone, zone_percent);
        }

        println!("\nЗанятые ячейки:");
        for (_, cell) in &self.cells {
            println!("{}: {} ({})", cell.address, cell.product, cell.quantity);
        }

        let total_cells = self.zones * self.shelves_per_zone * self.sections_per_shelf * self.shelves_per_section;
        let empty_cells = total_cells - self.cells.len() as i32;
        println!("\nПустые ячейки: {}", empty_cells);
    }
}

fn main() {
    let mut warehouse = Warehouse::new(1, 10, 7, 4, 2800);

    let mut input = String::new();
    loop {
        print!(">>> ");
        io::Write::flush(&mut io::stdout()).unwrap();
        input.clear();
        io::stdin().read_line(&mut input).unwrap();
        let parts: Vec<&str> = input.trim().split_whitespace().collect();
        
        if parts.is_empty() {
            continue;
        }

        match parts[0] {
            "ADD" => {
                if parts.len() != 4 {
                    println!("Неверное количество аргументов для ADD");
                    continue;
                }
                let product = parts[1];
                let quantity = match parts[2].parse() {
                    Ok(num) => num,
                    Err(_) => {
                        println!("Неверное количество");
                        continue;
                    }
                };
                let address = parts[3];
                warehouse.add_product(product, quantity, address);
            },
            "REMOVE" => {
                if parts.len() != 4 {
                    println!("Неверное количество аргументов для REMOVE");
                    continue;
                }
                let product = parts[1];
                let quantity = match parts[2].parse() {
                    Ok(num) => num,
                    Err(_) => {
                        println!("Неверное количество");
                        continue;
                    }
                };
                let address = parts[3];
                warehouse.remove_product(product, quantity, address);
            },
            "INFO" => {
                warehouse.print_info();
            },
            "EXIT" => {
                break;
            },
            _ => {
                println!("Неизвестная команда. Доступные команды: ADD, REMOVE, INFO, EXIT");
            }
        }
    }
}
