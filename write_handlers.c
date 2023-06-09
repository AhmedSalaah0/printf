#include "main.h"


/**
 * handle_write_char - Prints a string
 * @c: char types.
 * @buffer: Buffer array to handle print
 * @flags:  Calculates active flags.
 * @width: get width.
 * @precision: precision specifier
 * @size: Size specifier
 *
 * Return: Number of chars printed.
 */
int handle_write_char(char c, char buffer[],
	int flags, int width, int precision, int size)
{
	int l = 0;
	char dd = ' ';

	UNUSED(precision);
	UNUSED(size);

	if (flags & F_ZERO)
		dd = '0';

	buffer[l++] = c;
	buffer[l] = '\0';

	if (width > 1)
	{
		buffer[BUFF_SIZE - 1] = '\0';
		for (l = 0; l < width - 1; l++)
			buffer[BUFF_SIZE - l - 2] = dd;

		if (flags & F_MINUS)
			return (write(1, &buffer[0], 1) +
					write(1, &buffer[BUFF_SIZE - l - 1], width - 1));
		else
			return (write(1, &buffer[BUFF_SIZE - l - 1], width - 1) +
					write(1, &buffer[0], 1));
	}

	return (write(1, &buffer[0], 1));
}


/**
 * write_number - Prints a string
 * @is_negative: Lista of arguments
 * @ind: char types.
 * @buffer: Buffer array to handle print
 * @flags:  Calculates active flags
 * @width: get width.
 * @precision: precision specifier
 * @size: Size specifier
 *
 * Return: Number of chars printed.
 */
int write_number(int is_negative, int ind, char buffer[],
	int flags, int width, int precision, int size)
{
	int l = BUFF_SIZE - ind - 1;
	char dd = ' ', exc = 0;

	UNUSED(size);

	if ((flags & F_ZERO) && !(flags & F_MINUS))
		dd = '0';
	if (is_negative)
		exc = '-';
	else if (flags & F_PLUS)
		exc = '+';
	else if (flags & F_SPACE)
		exc = ' ';

	return (write_num(ind, buffer, flags, width, precision,
		l, dd, exc));
}

/**
 * write_num - Write using buffer number
 * @ind: Index at which in the buffer
 * @buffer: Buffer
 * @flags: Flags
 * @width: width
 * @prec: Precision
 * @length: length
 * @padd: Pading
 * @extra_c: new chars
 *
 * Return: printed char
 */
int write_num(int ind, char buffer[],
	int flags, int width, int prec,
	int length, char padd, char extra_c)
{
	int i, ddstrt = 1;

	if (prec == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0' && width == 0)
		return (0);
	if (prec == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
		buffer[ind] = padd = ' ';
	if (prec > 0 && prec < length)
		padd = ' ';
	while (prec > length)
		buffer[--ind] = '0', length++;
	if (extra_c != 0)
		length++;
	if (width > length)
	{
		for (i = 1; i < width - length + 1; i++)
			buffer[i] = padd;
		buffer[i] = '\0';
		if (flags & F_MINUS && padd == ' ')
		{
			if (extra_c)
				buffer[--ind] = extra_c;
			return (write(1, &buffer[ind], length) + write(1, &buffer[1], i - 1));
		}
		else if (!(flags & F_MINUS) && padd == ' ')
		{
			if (extra_c)
				buffer[--ind] = extra_c;
			return (write(1, &buffer[1], i - 1) + write(1, &buffer[ind], length));
		}
		else if (!(flags & F_MINUS) && padd == '0')/* extra char to left of padd */
		{
			if (extra_c)
				buffer[--ddstrt] = extra_c;
			return (write(1, &buffer[ddstrt], i - ddstrt) +
				write(1, &buffer[ind], length - (1 - ddstrt)));
		}
	}
	if (extra_c)
		buffer[--ind] = extra_c;
	return (write(1, &buffer[ind], length));
}

/**
 * write_unsgnd - Writes unsigned num
 * @is_negative: Number of indicating
 * @ind: start Index
 * @buffer: Array
 * @flags: Flags
 * @width: Width
 * @precision: Precision
 * @size: specifier size
 *
 * Return: chars num
 */
int write_unsgnd(int is_negative, int ind,
	char buffer[],
	int flags, int width, int precision, int size)
{
	int l = BUFF_SIZE - ind - 1, i = 0;
	char dd = ' ';

	UNUSED(is_negative);
	UNUSED(size);

	if (precision == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
		return (0);

	if (precision > 0 && precision < l)
		dd = ' ';

	while (precision > l)
	{
		buffer[--ind] = '0';
		l++;
	}

	if ((flags & F_ZERO) && !(flags & F_MINUS))
		dd = '0';

	if (width > l)
	{
		for (i = 0; i < width - l; i++)
			buffer[i] = dd;

		buffer[i] = '\0';

		if (flags & F_MINUS)
		{
			return (write(1, &buffer[ind], l) + write(1, &buffer[0], i));
		}
		else
		{
			return (write(1, &buffer[0], i) + write(1, &buffer[ind], l));
		}
	}

	return (write(1, &buffer[ind], l));
}

/**
 * write_pointer - Write memory address
 * @buffer: Arrays
 * @ind: start Index
 * @length: Length
 * @width: Width
 * @flags: Flags
 * @padd: padding char
 * @extra_c: extra char
 * @padd_start: Index
 *
 * Return: Number of chars
 */
int write_pointer(char buffer[], int ind, int length,
	int width, int flags, char padd, char extra_c, int padd_start)
{
	int i;

	if (width > length)
	{
		for (i = 3; i < width - length + 3; i++)
			buffer[i] = padd;
		buffer[i] = '\0';
		if (flags & F_MINUS && padd == ' ')
		{
			buffer[--ind] = 'x';
			buffer[--ind] = '0';
			if (extra_c)
				buffer[--ind] = extra_c;
			return (write(1, &buffer[ind], length) + write(1, &buffer[3], i - 3));
		}
		else if (!(flags & F_MINUS) && padd == ' ')
		{
			buffer[--ind] = 'x';
			buffer[--ind] = '0';
			if (extra_c)
				buffer[--ind] = extra_c;
			return (write(1, &buffer[3], i - 3) + write(1, &buffer[ind], length));
		}
		else if (!(flags & F_MINUS) && padd == '0')
		{
			if (extra_c)
				buffer[--padd_start] = extra_c;
			buffer[1] = '0';
			buffer[2] = 'x';
			return (write(1, &buffer[padd_start], i - padd_start) +
				write(1, &buffer[ind], length - (1 - padd_start) - 2));
		}
	}
	buffer[--ind] = 'x';
	buffer[--ind] = '0';
	if (extra_c)
		buffer[--ind] = extra_c;
	return (write(1, &buffer[ind], BUFF_SIZE - ind - 1));
}
