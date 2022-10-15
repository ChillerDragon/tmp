#include <cstdio>
#include <cstring>
#include <cstdlib>

void str_hex(char *dst, int dst_size, const void *data, int data_size)
{
	static const char hex[] = "0123456789ABCDEF";
	int data_index;
	int dst_index;
	for(data_index = 0, dst_index = 0; data_index < data_size && dst_index < dst_size - 3; data_index++)
	{
		dst[data_index * 3] = hex[((const unsigned char *)data)[data_index] >> 4];
		dst[data_index * 3 + 1] = hex[((const unsigned char *)data)[data_index] & 0xf];
		dst[data_index * 3 + 2] = ' ';
		dst_index += 3;
	}
	dst[dst_index] = '\0';
}

void str_format(char *buffer, int buffer_size, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

#if defined(CONF_FAMILY_WINDOWS) && !defined(__GNUC__)
	_vsprintf_p(buffer, buffer_size, format, ap);
#else
	vsnprintf(buffer, buffer_size, format, ap);
#endif

	va_end(ap);

	buffer[buffer_size-1] = 0; /* assure null termination */
}

void str_bin(char *dst, int dst_size, const void *data, int data_size)
{
	int i;
	for (i = 0; i < data_size; i++)
	{
		if(i + 1 * 9 > dst_size)
			break;
		str_format(dst + i * 9, 9, "%08b ", ((unsigned char *)data)[i]);
		dst[-1 + i * 9] = ' '; // delete null terminator
	}
	dst[-1 + i * 9] = '\0';
}

void print_bytes(const void *pData, int Size)
{
	printf("%p %d\n", pData, *(int *)pData);
	char aHex[128];
	str_hex(aHex, sizeof(aHex), pData, Size);
	puts(aHex);
	str_bin(aHex, sizeof(aHex), pData, Size);
	puts(aHex);
}

int main()
{
	puts("=================");
	int x = 255;
	int *p = &x;
	const void *pData = p;
	print_bytes(pData, 4);
}

