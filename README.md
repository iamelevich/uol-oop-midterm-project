# Object Oriented Programming Midterm Assessment

## Requirements

Compiler that support c++17 (At least some of the futures).

### Tested on

- Compiler:
    - Apple clang version 14.0.0 (clang-1400.0.29.202)
    - Target: arm64-apple-darwin22.2.0
    - Thread model: posix

## Checklist

- [x] Command parsing code
    - Bot.cpp:318 - split user input by space and try to interpretate command.
- [x] Command: `help`
    - Bot.cpp:25 - implementation
    - Bot.cpp:328 - parsing
- [x] Command: `help <cmd>`
    - Bot.cpp:31 - implementation
    - Bot.cpp:328 - parsing
- [x] Command: `prod`
    - Bot.cpp:133 - implementation
    - Bot.cpp:339 - parsing
    - OrderBook.cpp:15 and OrderBook.cpp:21 - using set to store all available products
- [x] Command: `min`
    - Bot.cpp:142 - implementation
    - Bot.cpp:346 - parsing
- [x] Command: `max`
    - Bot.cpp:158 - implementation
    - Bot.cpp:353 - parsing
- [x] Command: `avg`
    - Bot.cpp:174 - implementation
    - Bot.cpp:360 - parsing
- [x] Command: `predict`
    - Bot.cpp:236 - implementation
    - Bot.cpp:374 - parsing
- [x] Command: `time`
    - Bot.cpp:278 - implementation
    - Bot.cpp:383 - parsing
- [x] Command: `step`
    - Bot.cpp:283 - implementation
    - Bot.cpp:390 - parsing
- [x] Implement your own command
    - Implemented commands: `mean`, `exit` and extended `step` to support jump over more than one time step
        - `mean`
            - Bot.cpp:205 - implementation
            - Bot.cpp:367 - parsing
        - `exit`
            - Bot.cpp:402 - parsing and implementation
        - `step <jump>`
            - Bot.cpp:289 - implementation
            - Bot.cpp:390 - parsing
- [x] Optimise the exchange code
    - Probably I got something wrong, but I didn't find any exchange code, also none of commands do not do any exchange
      operations. So I decide to optimize csv parsing code. Template implementation took ~3400ms on my computer to parse
      and load provided CSV file. After some refactor and using of some c++17 features now it's using ~1600-1700ms, so
      twice faster. Definitely this code wouldn't work on the big files, but with big files we wouldn't store DB in
      memory, so for current requirements as study project I think it's okay. Unfortunately Clang on my Mac still have
      no support of execution::par and it's definitely better to parallel csv parsing here, but I've decided not spend
      more time on this without any efforts. In real life I will use library for that or will use GoLang or Rust,
      because it's just easier to write and maintain parallel code.
    - CSVReader.cpp:14 - implementation