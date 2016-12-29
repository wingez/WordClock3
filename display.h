







class Display
{
public:


	Display();


	void SetRaw(unsigned char value);
	void Clear();

	void Show();



	static void Setup();


private:
	unsigned long data;
	
	static void shiftout(unsigned long *val);

	//static unsigned char brightness;

};


