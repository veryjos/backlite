#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

// Print usage
int usage(char* p_name) {
	fprintf(stderr, "Usage:\n  %s backlight_path +x; # Increment backlight by x%%\n  %s backlight_path -x; # Decrement backlight by x%%\n  %s backlight_path  x; # Set backlight to x%%\n", p_name, p_name, p_name);	

	return 1;
}

// Simple read of a file to a destination buffer
int simple_read(char* file_path, unsigned char* dest, int dest_size) {
	FILE *f = fopen(file_path, "r");

	if (f == 0)
		return 1;

	fread(dest, dest_size, dest_size, f);
	
	fclose(f);

	return 0;
}

// Simple write of a file to a destination buffer
int simple_write(char* file_path, unsigned char* src) {
	FILE *f = fopen(file_path, "w");

	if (f == 0)
		return 1;

	fprintf(f, "%s", src);

	fclose(f);
}

int set_brightness(char* file_path, int value) {
	char value_str[1024];

	sprintf(value_str, "%d", value);

	return simple_write(file_path, value_str);
}

int main(int argc, char* argv[]) {
	char operation;
	char* backlight_path;
	char* param;
	char* c;
	char read_dst[1024], max_backlight_path[PATH_MAX], cur_backlight_path[PATH_MAX];
	
	int max_brightness, desired_brightness;

	if (argc < 3) {
		return usage(argv[0]);
	}

	backlight_path = argv[1];
	operation = argv[2][0];
	param = argv[2];

	if (operation == '+' || operation == '-') {
		// Operation is prefixed, so skip the first character
		param++;

		// Check if the string actually has anything past the operation
		if (*param == '\0') {
			return usage(argv[0]);
		}
	} else {
		// Not using an operation, so just set it to 0
		operation = 0;
	}

	// Check that param is all numbers
	c = param;
	while (*c != 0) {
		if (*c < '0' || *c > '9')
			return usage(argv[0]);

		++c;
	}

	// Get max brightness of screen
	strcpy(max_backlight_path, backlight_path);
	strcat(max_backlight_path, "/max_brightness");
	if (simple_read(max_backlight_path, read_dst, sizeof(read_dst)) != 0) {
		fprintf(stderr, "Failed to get backlight max brightness at %s\n", max_backlight_path);

		return 1;
	}

	// Trusting our input and using atoi here
	max_brightness = atoi(read_dst);


	// Get path to current brightness of screen
	strcpy(cur_backlight_path, backlight_path);
	strcat(cur_backlight_path, "/brightness");

	// We only need the current value if using a relative operation
	// Check if the operation is used
	if (operation != 0) {
		int cur_brightness, delta;
		if (simple_read(cur_backlight_path, read_dst, sizeof(read_dst)) != 0) {
			fprintf(stderr, "Failed to get backlight current brightness at %s\n", max_backlight_path);
	
			return 1;
		}
	
		// Get current brightness
		cur_brightness = atoi(read_dst);

		// Get the delta in percentage as a delta in brightness units
		delta = (float)atoi(param) / 100 * max_brightness;

		// If decrement, set the delta to negative
		if (operation == '-')
			delta = -delta;

		// Calculate new desired value
		desired_brightness = cur_brightness + delta;
	} else {
		// Convert percentage as specified by user to value relative to max brightness
		desired_brightness = (float)atoi(param) / 100 * max_brightness;
	}

	// Check that we didn't exceed bounds
	if (desired_brightness < 0)
		desired_brightness = 0;
	else if (desired_brightness > max_brightness)
		desired_brightness = max_brightness;
	
	if (set_brightness(cur_backlight_path, desired_brightness) != 0) {
		fprintf(stderr, "Failed to set brightness! You may need sudo.\n");

		return 1;
	}

	return 0;
}
