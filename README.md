# Pipex

A program that simulates the shell pipeline behavior, implementing pipes `|`, file redirection `>` or `<` and command execution. The bonus has multiple commands, `>>` or `<<` and heredoc 

## Usage

1. Clone the repository
```sh
git clone https://github.com/andrexandre/pipex.git ; cd pipex
```

2. Compile the program
```sh
make
```

3. Run the prorgam
```sh
./pipex infile 'grep l' 'head -n 1' outfile
```

The following syntax:

`./pipex infile cmd1 cmd2 outfile`

Is equivalent to:

`< infile cmd1 | cmd2 > outfile`

## Bonus

Compile the program
```sh
make bonus
```

For multiple commands:

`./pipex infile cmd1 cmd2 cmd3 ... outfile`

For here_doc functionality:

`./pipex here_doc DELIMITER cmd1 cmd2 outfile`

Equivalent to:

`cmd1 << DELIMITER | cmd2 >> outfile`

> [!NOTE]
> This project is part of the 42 School curriculum
