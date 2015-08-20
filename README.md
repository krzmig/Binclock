# binclock
It's my very first "project" in C++ so feel free to say me that i made thousands of stupid things in code.
I don't like simple programs with big libraries included to make something in 5 lines than 10, so this one using only standard C++ libraries.
I made it because I wanted something to display binary clock in Conky, but I didn't found nothing special, so I decided to make it by myself. In other hand I wanted to increase my C++ level from -1 to 1 in 1-100 scale, so I dropped this project in PHP, and started from 0 in C++. This is end of this boring story.
Oh, and sorry for my english.
## Requirements
* unix system
* g++ compiler
* console with utf8 support (optional)
## Instalation
* download file *binclock.cpp*
* *cd* to folder with downloaded file
* run: *$ g++ ./binclock.cpp -o binclock*
* optional - to make global program link:
	*# ln -sr binclock /usr/local/bin/binclock*
* or just download and run: *# sh install.sh* to make this all in one command
## Usage
*binclock [options]*
run *binclock --help* to get this help message (i know, i'm lazy):
```  --description      - Showing description for clock (G is 16, Z is 32).
  --truechar <char>  - String to show for 1 bit, default: "◉ ".
  --falsechar <char> - String to show for 0 bit, default: "○ ".
  --live             - Showing live clock (refresh every second). For hex time there is some delay,
                       because one hexadecimal seconds it's equal to 1.318359375 standard seconds,
                       and refresh interval is one standard second.
  --vertical         - Showing clock in vertical mode.
  --decimal          - Use decimal clock mode (not working with --hex option).
  --bcd              - Alias for --decimal.
  --noseconds        - Don't show seconds.
  --12hours          - Show clock in 12h mode, default is 24h mode (not working with --hex option).
  --hex              - Show clock for Hexadecimal time (16h, 256m, 16s).
                       See: https://en.wikipedia.org/wiki/Hexadecimal_time
  --help             - Show this help.```
## Todo
- [ ] short console params like -d 
## Example outputs
```$ binclock --description
  Z G 8 4 2 1 
H   ◉ ○ ◉ ○ ◉ 
M ○ ○ ◉ ◉ ○ ◉ 
S ◉ ○ ◉ ◉ ○ ○```
```$ binclock --vertical
  ○ ○ 
◉ ○ ○ 
○ ◉ ○ 
◉ ◉ ◉ 
○ ◉ ◉ 
◉ ◉ ◉ ```
```$ binclock --decimal --12hours --vertical --description
  H H M M S S 
8   ◉   ○   ○ 
4   ○ ○ ◉ ○ ◉ 
2 ○ ○ ○ ○ ◉ ○ 
1 ○ ◉ ◉ ○ ◉ ○ ```
And of course for real nerds hexadecimal:
```$ binclock --hex --description
  8 4 2 1 
H ◉ ◉ ◉ ○ 
M ○ ○ ◉ ○ 
M ◉ ◉ ○ ◉ 
S ◉ ○ ◉ ◉```

