int main()
{
	int a,b,c,d;
	a=1;
	b=0;
	c=0;
	d=1;
	while(a<10)
	{
		a = a +1 ;
		while(b<5)
		{
			b = b +1 ;
			while(c<20)
			{
				d=d+1;
				c = c + 1;
			}
		}
	}
	return d;
}