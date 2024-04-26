int var_c = 2;

int funcion_c(int i, int j)
{
	return i + j;
}

extern int var_s;
extern int sbr_s(int, int);

int main()
{
	int k;

	k = var_s + 1;
	var_c = sbr_s(3, k);

	while (1)
		;
}