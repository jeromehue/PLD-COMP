int main()
{
	int a,b,c,d;
	a=1;
	b=0;
	c=0;
	d=1;
	while(a<10)
	{
		if(a>=6)
		{
			a = a + 1;
			while(b<5)
			{
				b = b + 1;
				d=d+1;
			}
		}
		else
		{
			a = a + 1;
			d=d+1;
		}
	}
	return d;
}
