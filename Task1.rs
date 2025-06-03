use std::collections::HashMap;
use std::io::{self, Write};

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

    fn address_valid(&self, address: &str) -> bool {
        if address.len() != 5 {
            return false;
        }
        
        let zone = address.chars().nth(0).unwrap();
        if zone < 'A' || zone >= ('A' as u8 + self.zones as u8) as char {
            return false;
        }
        
        let shelf_part = &address[1..3];
        let shelf = match shelf_part.parse::<i32>() {
            Ok(num) => num,
            Err(_) => return false,
        };
        if shelf < 1 || shelf > self.shelves_per_zone {
            return false;
        }
        
        let section = match address.chars().nth(3).unwrap().to_digit(10) {
            Some(num) => num as i32,
            None => return false,
        };
        if section < 1 || section > self.sections_per_shelf {
            return false;
        }
        
        let shelf_num = match address.chars().nth(4).unwrap().to_digit(10) {
            Some(num) => num as i32,
            None => return false,
        };
        if shelf_num < 1 || shelf_num > self.shelves_per_section {
            return false;
        }
        
        true
    }

    fn add(&mut self, product: &str, quantity: i32, address: &str) {
        if !self.address_valid(address) {
            println!("Ошибка: Неверный адрес: {}", address);
            return;
        }

        if quantity > 10 {
            println!("Ошибка: Нельзя добавить более 10 единиц в ячейку");
            return;
        }

        if let Some(cell) = self.cells.get_mut(address) {
            if cell.product != product {
                println!("Ошибка: Ячейка {} уже содержит {}", address, cell.product);
                return;
            }
            if cell.quantity + quantity > 10 {
                println!("Ошибка: Ячейка {} не может содержать более 10 единиц (сейчас: {})", 
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

    fn remove(&mut self, product: &str, quantity: i32, address: &str) {
        if !self.address_valid(address) {
            println!("Ошибка: Неверный адрес: {}", address);
            return;
        }

        if !self.cells.contains_key(address) {
            println!("Ошибка: Ячейка {} пуста", address);
            return;
        }

        let cell = self.cells.get_mut(address).unwrap();
        if cell.product != product {
            println!("Ошибка: Ячейка {} содержит {}, а не {}", 
                address, cell.product, product);
            return;
        }

        if cell.quantity < quantity {
            println!("Ошибка: Недостаточно {} в ячейке {} (доступно: {})", 
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

    fn info(&self) {
        let total_percent = (self.used_capacity as f64 / self.total_capacity as f64) * 100.0;
        println!("Информация о складе:");
        println!("Общая заполненность: {:.1}%", total_percent);

        let mut zone_capacity = HashMap::new();
        let mut zone_used = HashMap::new();
        
        for zone in 'A'..('A' as u8 + self.zones as u8) as char {
            zone_capacity.insert(zone, 
                self.shelves_per_zone * self.sections_per_shelf * self.shelves_per_section * 10);
            zone_used.insert(zone, 0);
        }

        for (_, cell) in &self.cells {
            let zone = cell.address.chars().next().unwrap();
            *zone_used.get_mut(&zone).unwrap() += cell.quantity;
        }

        for zone in 'A'..('A' as u8 + self.zones as u8) as char {
            let percent = (*zone_used.get(&zone).unwrap() as f64 / 
                          *zone_capacity.get(&zone).unwrap() as f64) * 100.0;
            println!("Зона {} заполнена на {:.1}%", zone, percent);
        }

        println!("\nЗанятые ячейки:");
        for (_, cell) in &self.cells {
            println!("{}: {} ({})", cell.address, cell.product, cell.quantity);
        }

        let total_cells = self.zones * self.shelves_per_zone * 
                         self.sections_per_shelf * self.shelves_per_section;
        let empty_cells = total_cells - self.cells.len() as i32;
        println!("\nПустые ячейки: {}", empty_cells);
    }
}

fn main() {
    let mut warehouse = Warehouse::new(1, 10, 7, 4, 2800);
    
    loop {
        print!(">>> ");
        io::stdout().flush().unwrap();
        
        let mut command = String::new();
        io::stdin().read_line(&mut command).unwrap();
        let command = command.trim();
        
        if command.is_empty() {
            continue;
        }

        let parts: Vec<&str> = command.split_whitespace().collect();
        match parts[0] {
            "ADD" => {
                if parts.len() < 4 {
                    println!("Ошибка: Неправильный формат команды ADD. Используйте: ADD <продукт> <количество> <адрес>");
                    continue;
                }
                
                let product = parts[1];
                let quantity = match parts[2].parse::<i32>() {
                    Ok(num) => num,
                    Err(_) => {
                        println!("Ошибка: Неправильный формат количества. Должно быть целое число");
                        continue;
                    }
                };
                let address = parts[3];
                
                warehouse.add(product, quantity, address);
            },
            "REMOVE" => {
                if parts.len() < 4 {
                    println!("Ошибка: Неправильный формат команды REMOVE. Используйте: REMOVE <продукт> <количество> <адрес>");
                    continue;
                }
                
                let product = parts[1];
                let quantity = match parts[2].parse::<i32>() {
                    Ok(num) => num,
                    Err(_) => {
                        println!("Ошибка: Неправильный формат количества. Должно быть целое число");
                        continue;
                    }
                };
                let address = parts[3];
                
                warehouse.remove(product, quantity, address);
            },
            "INFO" => {
                warehouse.info();
            },
            "EXIT" => {
                break;
            },
            _ => {
                println!("Ошибка: Неизвестная команда '{}'. Доступные команды: ADD, REMOVE, INFO, EXIT", parts[0]);
            }
        }
    }
}
