# Compiler to use
CC = clang

# Compiler flags
CFLAGS = -Iheaders # Include the headers directory

# Source files
SRC = add.c commands.c dialogue.c ems.c initialise.c print.c remove.c main.c

# Output executable name
TARGET = eotf

# Default rule: build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Optional rule for object files (for larger projects)
# %.o: %.c
#     $(CC) $(CFLAGS) -c $< -o $@

# Clean rule: remove the executable
clean:
	rm -f $(TARGET)

.PHONY: clean
