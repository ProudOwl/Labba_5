use std::collections::VecDeque;
use std::io;

#[derive(Clone)]
struct Visitor {
    ticket: String,
    duration: i32,
}

struct Window {
    total_time: i32,
    visitors: Vec<Visitor>,
}

impl Window {
    fn new() -> Self {
        Window {
            total_time: 0,
            visitors: Vec::new(),
        }
    }
}

fn generate_ticket(counter: &mut i32) -> String {
    *counter += 1;
    format!("T{:03}", counter)
}

fn distribute_queue(q: &mut VecDeque<Visitor>, windows_count: usize) -> Vec<Window> {
    let mut windows = Vec::with_capacity(windows_count);
    for _ in 0..windows_count {
        windows.push(Window::new());
    }

    while let Some(visitor) = q.pop_front() {
        let duration = visitor.duration;  // Сохраняем duration до перемещения visitor
        let min_window = windows
            .iter_mut()
            .min_by_key(|w| w.total_time)
            .unwrap();

        min_window.visitors.push(visitor);
        min_window.total_time += duration;  // Используем сохраненное значение
    }
    windows
}

fn main() {
    let mut ticket_counter = 0;
    let mut visitors_queue = VecDeque::new();

    println!(">>> Введите кол-во окон");
    print!("<<< ");
    io::Write::flush(&mut io::stdout()).unwrap();

    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    let windows_count: usize = input.trim().parse().unwrap();

    loop {
        print!("<<< ");
        io::Write::flush(&mut io::stdout()).unwrap();
        input.clear();
        io::stdin().read_line(&mut input).unwrap();
        let parts: Vec<&str> = input.trim().split_whitespace().collect();

        if parts.is_empty() {
            continue;
        }

        match parts[0] {
            "ENQUEUE" => {
                if parts.len() < 2 {
                    println!("Необходимо указать длительность");
                    continue;
                }
                let duration: i32 = parts[1].parse().unwrap();
                let ticket = generate_ticket(&mut ticket_counter);
                visitors_queue.push_back(Visitor { ticket: ticket.clone(), duration });
                println!(">>> {}", ticket);
            }
            "DISTRIBUTE" => {
                break;
            }
            _ => {
                println!("Неизвестная команда");
            }
        }
    }

    let windows = distribute_queue(&mut visitors_queue, windows_count);

    for (i, window) in windows.iter().enumerate() {
        print!(">>> Окно {} ({} минут): ", i + 1, window.total_time);
        for (j, visitor) in window.visitors.iter().enumerate() {
            if j != 0 {
                print!(", ");
            }
            print!("{}", visitor.ticket);
        }
        println!();
    }
}
