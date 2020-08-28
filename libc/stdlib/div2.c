// https://stackoverflow.com/a/2422722
int div2(int numer, int denom) {
	if (!denom) return 0;
	return (numer + (denom - 1)) / denom;
}
