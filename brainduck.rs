use std::env;
use std::fs;
use std::io::{self, Read};
use std::time::Instant;

fn showmem(mem: Vec<u8>) {
    println!("\nMemory dump:");

    let chunk_size = 8;

    for (i, chunk) in mem.chunks(chunk_size).enumerate() {
        // offset
        print!("{:08x}  ", i * chunk_size);

        // hex bytes
        for b in chunk {
            print!("{:02x} ", b);
        }

        // padding for shorter last line
        for _ in 0..(chunk_size - chunk.len()) {
            print!("   ");
        }

        print!("  ");

        // ASCII (safe)
        for &b in chunk {
            let ch = if b.is_ascii_graphic() || b == b' ' {
                b as char
            } else {
                '.'
            };
            print!("{}", ch);
        }

        println!();
    }
}

fn bf(code: Vec<char>) {
    let start = Instant::now();

    let mut tape = vec![0u8; 64];
    let mut ptr = 0usize;
    let mut max_ptr = 0_usize;

    // build jump table
    let mut stack = vec![];
    let mut jump = vec![0usize; code.len()];
    for (i, &c) in code.iter().enumerate() {
        if c == '[' {
            stack.push(i);
        }
        if c == ']' {
            let j = stack.pop().unwrap();
            jump[i] = j;
            jump[j] = i;
        }
    }

    let mut ip = 0usize;
    while ip < code.len() {
        match code[ip] {
            '>' => {
                ptr += 1;
                max_ptr = max_ptr.max(ptr)
            }
            '<' => ptr -= 1,
            '+' => tape[ptr] = tape[ptr].wrapping_add(1),
            '-' => tape[ptr] = tape[ptr].wrapping_sub(1),
            '.' => print!("{}", tape[ptr] as char),
            ',' => tape[ptr] = io::stdin().bytes().next().unwrap().unwrap(),
            '[' if tape[ptr] == 0 => ip = jump[ip],
            ']' if tape[ptr] != 0 => ip = jump[ip],
            _ => {}
        }
        ip += 1;
    }

    let elapsed = start.elapsed();

    println!("\n---");

    println!("execution time = {:?}", elapsed);
    println!("max memory usage = {:x} bytes", max_ptr + 1);

    showmem(tape);
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        eprintln!("Usage: {} <file_path>", args[0]);
        return;
    }

    let file_path = &args[1];

    let prog: Vec<char> = fs::read_to_string(file_path)
        .expect("Could not read file")
        .chars()
        .collect();

    bf(prog);
}
