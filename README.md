Mimi, a terMInal reMInder.

# Build

## Linux

Run `make`.
Compiled file path is `./bld/main`.

# Usage

## Setup

Mimi is designed to be used in a terminal, specifically appended to your shell config to be ran everytime the terminal launches.

Example in **fish** shell, `config.fish`:

```fish
/usr/bin/mimi
```

This will run when you launch your terminal and display due reminders.

## Basics

All commands (`remind`, `delete`, `list`, `help`) have short forms: `r` for `remind` and etc.

### Creating a reminder

Create a reminder:

```bash
$ mimi remind "My reminder" 1m
```
This creates a reminder that will go off in 1 minute.

- s, is second
- m, is minute
- h, is hour
- d, is day
- w, is week
- o, is month
- y, is year

If no time format is specified:

```bash
$ mimi remind "Another reminder" 10
```

The default is seconds.

### Deleting a reminder

To delete a reminder:

```bash
$ mimi delete 0
```

Here zero is the index of the reminder. To get the index use the `list` command.

### Listing reminders

To list reminders:

```bash
$ mimi list
```
