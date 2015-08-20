#include <ctime>
#include <iostream>
#include <string>
#include <cstring> // strcmp
#include <cstdlib> // atoi and exit
#include <cmath> // for floor
#include <unistd.h> // for sleep

using namespace std;

/**
 * Binary, console clock program, with hexadecimal time for real nerds.
 * 
 * It's my very first "project" in c++, so it can be piece of shit. You running this on you own risk!
 * Only standard libs inside. You need only g++ to compile it. Oh and Linux... Sorry Windows users... but to be honest i don't know is there anybody who would like to use console clock in Windows. Give me sign if is any, only for information, I don't want install Windows to upgrade this program :(.
 * You can use this program to show clock in Conky.
 * Have fun.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 * @licence GNU GPL v2.0
 *
 * @todo short console params like -d
 */
class binaryClock
{
	protected:
		string true_char; // string for true bits
		string false_char; // string for false bits
		string null_char; // string for free space
		bool show_descriptions;
		bool run_live;
		bool show_decimal;
		bool vertical;
		bool show_seconds;
		bool twelve_hours;
		bool hex_time; 
		
		string ch_time; // hhmmss for decimal clock 
		int fulltime[4]; // h, m, s (h,m,m,s for hex)
		bool bitTime[6][6]; // array for binary presentation of time
		int bitTimeNulls[6]; // array to ignore first bits in any line
		string bitTimeDesc; // string with description like: H H M M S S
		string description; // string with description of bits value, ex: 8 4 2 1
		int lines_counter; // line counter for live clock
		
	public:
		binaryClock();
		void run();
		void help();
		void checkParam( char *, char * );
	
	protected:
		void setCurrentTime();
		void setOptions();
		void drawDescription();
		void drawClock();
		void drawVerticalClock();
		void drawNewLine();
		void clearConsole();
		void generateBitArray();
		void genNullChar();
		void intToBits( int, bool [], int );
		void drawBits( bool [], int, int );
		string fillSpaces( string, int );
		string fillSpaces( char, int );
		int utf8_strlen( const string& );
};

/**
 * Constructor to setup thing which I can't do in declaration...
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
binaryClock::binaryClock()
{
	this -> true_char = "◉ ";
	this -> false_char = "○ ";
	this -> genNullChar();
	this -> vertical = false;
	this -> show_decimal = false;
	this -> run_live = false;
	this -> show_seconds = true;
	this -> show_descriptions = false;
	this -> twelve_hours = false;
	this -> hex_time = false;
	
	this-> lines_counter = 0;
}

/**
 * Method to display help command
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::help()
{
	cout << "binclock [OPTIONS]\n\n" <<
		"  --description      - Showing description for clock (G is 16, Z is 32).\n" <<
		"  --truechar <char>  - String to show for 1 bit, default: \"◉ \".\n" <<
		"  --falsechar <char> - String to show for 0 bit, default: \"○ \".\n" <<
		"  --live             - Showing live clock (refresh every second). For hex time there is some delay,\n" << 
		"                       because one hexadecimal seconds it's equal to 1.318359375 standard seconds,\n" <<
		"                       and refresh interval is one standard second.\n" <<
		"  --vertical         - Showing clock in vertical mode.\n" <<
		"  --decimal          - Use decimal clock mode (not working with --hex option).\n" <<
		"  --bcd              - Alias for --decimal.\n" <<
		"  --noseconds        - Don't show seconds.\n" <<
		"  --12hours          - Show clock in 12h mode, default is 24h mode (not working with --hex option).\n" <<
		"  --hex              - Show clock for Hexadecimal time (16h, 256m, 16s).\n" <<
		"                       See: https://en.wikipedia.org/wiki/Hexadecimal_time\n" <<
		"  --help             - Show this help.\n";
	exit(0);
}

/**
 * Main method of class. This one starts others methods.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::run()
{
	this -> setOptions();
	do
	{
		this -> setCurrentTime();
		this -> generateBitArray();
		if( this -> show_descriptions )
			this -> drawDescription();
		if( this -> vertical )
			this -> drawVerticalClock();
		else
			this -> drawClock();
		if( this -> run_live )
			this -> clearConsole();
	}
	while( this -> run_live );
}

/**
 * Method sets options depend on params posted by user. 
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::setOptions()
{
	if( this -> hex_time )
	{
		this -> bitTimeDesc = "HMMS";
	}
	else if( this -> show_decimal )
	{
		this -> bitTimeNulls[0] = 2;
		this -> bitTimeNulls[2] = 1;
		this -> bitTimeNulls[4] = 1;
		this -> bitTimeDesc = "HHMMSS";
	}
	else
	{
		this -> bitTimeNulls[0] = 1;
		this -> bitTimeDesc = "HMS";
	}
	this -> description = "1248GZ";
}

/**
 * This method gets current time and convert them to version that will be used by others methods.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::setCurrentTime()
{	
	time_t timestamp = time(NULL);

	char hh[3];
	char mm[3];
	char ss[3];
	
	if(( this -> twelve_hours ) && ( !this -> hex_time ))
		strftime( hh, 3, "%I", localtime( &timestamp ));
	else
		strftime( hh, 3, "%H", localtime( &timestamp ));
	strftime( mm, 3, "%M", localtime( &timestamp ));
	strftime( ss, 3, "%S", localtime( &timestamp ));
	
	this -> fulltime[0] = atoi( hh );
	this -> fulltime[1] = atoi( mm );
	this -> fulltime[2] = atoi( ss );
	
	this -> ch_time = hh;
	this -> ch_time += mm;
	this -> ch_time += ss;

	if( this -> hex_time )
	{
		int today_timestamp = floor(( this -> fulltime[0] * 3600 + this -> fulltime[1] * 60 + this -> fulltime[2]) / 1.318359375 );
		this -> fulltime[0] = floor( today_timestamp / 4096 );
		today_timestamp = today_timestamp % 4096;
		this -> fulltime[1] = floor( today_timestamp / 256 );
		today_timestamp = today_timestamp % 256;
		this -> fulltime[2] = floor( today_timestamp / 16 );
		today_timestamp = today_timestamp % 16;
		this -> fulltime[3] = today_timestamp;
	}
}

/**
 * Method generate bits array for all needed units, like hours, seconds etc.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::generateBitArray()
{
	if( this -> hex_time )
	{
		for( int i = 0; i < 4; i++ )
		{
			this -> intToBits( this -> fulltime[i], this -> bitTime[i], 4 );
		}
	}
	else if( this -> show_decimal )
	{
		for( int i = 0; i < 6; i++ )
		{
			this -> intToBits( this -> ch_time[i], this -> bitTime[i], 4 );
		}
	}
	else
	{
		for( int i = 0; i < 3; i++ )
		{
			this -> intToBits( this -> fulltime[i], this -> bitTime[i], 6 );
		}
	}
}

/**
 * This method gets int value, convert it to binary and put results bits to given array.
 *
 * @param int number - Int value which will be converted to binary.
 * @param bool[] array - It's array to which method should puts results bits.
 * @param int size - This is length. For example if we want convert seconds, we set size 6 (6 bits are needed to make 60), but for 12 hours size 4 it's enough. 
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::intToBits( int number, bool array[], int size )
{
	for( int i = size - 1; i >= 0; i-- )
	{
		if( number > 0 )
		{
			if( number % 2 == 1 )
				array[i] = true;
			else
				array[i] = false;
			number = floor( number / 2 );
		}
		else
			array[i] = false;
	}
}

/**
 * Method draw one line of chars on screen depends on generated earlier bits arrays.
 *
 * @param bool[] array - It's array from which chars are drawn.
 * @param int size - This Is length. Max number of bits to show maximum possible value for this unit.
 * @param int nullsCount - Int value which defined how much first chars should be ignored. If hours take less place than minutes, it's nice to show free space than false bit.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::drawBits( bool array[], int size, int nullsCount = 0 )
{
	for( int i = 0; i < size; i++ )
	{
		if( i < nullsCount )
			cout << this -> null_char;
		else if( array[i] )
			cout << this -> true_char;
		else
			cout << this -> false_char;
	}
	this -> drawNewLine();
}

/**
 * Method to draw horizontal descriptions. 
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::drawDescription()
{
	int size = this -> null_char.length();
	cout << this -> fillSpaces( " ", size );

	if( this -> vertical )
	{
		int limit = 3;
		if( this -> hex_time )
			limit = 4;
		else if( this -> show_decimal )
			limit = 6;
		
		for( int i = 0; i < limit; i++ )
		{
			cout << this -> fillSpaces( this -> bitTimeDesc[i], size );
		}
		this -> drawNewLine();
	}
	else
	{
		int limit = 5;
		if( this -> hex_time )
			limit = 3;
		else if( this -> show_decimal )
			limit = 3;
		
		for( int i = limit; i >= 0; i-- )
		{
			cout << this -> fillSpaces( this -> description[i], size );
		}
		this -> drawNewLine();
	}
}

/**
 * Method to draw horizontal version of clock, with vertical descriptions if they are on. 
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::drawClock()
{
	int length = 6;
	int limit = 2;
	if( this -> show_seconds )
		limit = 3;
	int size = this -> null_char.length();
	
	if( this -> hex_time )
	{
		limit += 1;
		length = 4;
	}
	else if( this -> show_decimal )
	{
		limit += limit;	
		length = 4;
	}

	for( int i = 0; i < limit; i++ )
	{
		if( this -> show_descriptions )
			cout << this -> fillSpaces( this -> bitTimeDesc[i], size );
		this -> drawBits( this -> bitTime[i], length, this -> bitTimeNulls[i] );
	}
}

/**
 * Method to draw vertical version of clock with vertical descriptions if they are on.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::drawVerticalClock()
{
	int size = this -> null_char.length();
	int init_i = 5;
	int limit_j = 3;
	
	if( this -> hex_time )
	{
		init_i = 3;
		limit_j = 4; 
	}
	else if( this -> show_decimal )
	{
		init_i = 3;
		limit_j = 6;
	}

	for( int i = init_i, k = 0; i >= 0; i--, k++ )
	{
		if( this -> show_descriptions )
			cout << this -> fillSpaces( this -> description[i], size );
		for( int j = 0; j < limit_j; j++ )
		{
			if( this -> bitTimeNulls[j] > k )
				cout << this -> null_char;
			else if( bitTime[j][k] )
				cout << this -> true_char;
			else
				cout << this -> false_char;
		}	
		this -> drawNewLine();
	}
}

/**
 * Method draw new line to console, and count number of lines to display live clock in the same place as old clock.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::drawNewLine()
{
	cout << "\n";
	this -> lines_counter++;
}

/**
 * Back drawing cursor up for numbers of lines counted in drawNewLine method and to begin of line.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::clearConsole()
{
	sleep(1);
	for( int i = 0; i < this -> lines_counter; i++ )
	{
		cout << "\033[1A\033[K";
	}
	this -> lines_counter = 0;
}

/**
 * This method fill spaces after given text to given size. ("1", 3) will return "1  "
 *
 * @param string/char content - string to expand
 * @param int length - needed length of content
 * @return string/char
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
string binaryClock::fillSpaces( string content, int length )
{
	for( int i = content.size(); i < length; i++ )
	{
		content += ' ';
	}
	return content;
}
string binaryClock::fillSpaces( char ch, int length )
{
	string content = "";
	content += ch;
	for( int i = 1; i < length; i++ )
	{
		content += ' ';
	}
	return content;
}

/**
 * Generate string for null value with the same length as true string.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::genNullChar()
{
	for( int i = 0; i < this -> utf8_strlen( this -> true_char ); i++ )
	{
		this -> null_char += ' ';
	}
}

/**
 * Method to check console params and setup values depend on this params.
 *
 * @param char* param - Param name to set.
 * @param char* next_param - Next param, example needed for setting true/false string.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
void binaryClock::checkParam( char *param, char *next_param )
{
	if( strcmp( param, "--help" ) == 0 )
		this -> help();
	else if( strcmp( param, "--description" ) == 0 )
		this -> show_descriptions = true;
	else if( strcmp( param, "--live" ) == 0 )
		this -> run_live = true;
	else if( strcmp( param, "--vertical" ) == 0 )
		this -> vertical = true;
	else if(( strcmp( param, "--decimal" ) == 0 ) || ( strcmp( param, "--bcd" ) == 0 ))
		this -> show_decimal = true;
	else if( strcmp( param, "--noseconds" ) == 0 )
		this -> show_seconds = false;
	else if( strcmp( param, "--hex" ) == 0 )
		this -> hex_time = true;
	else if( strcmp( param, "--12hours" ) == 0 )
	{
		this -> twelve_hours = true;	
		this -> bitTimeNulls[0] = 2;
	}
	else if( strcmp( param, "--truechar" ) == 0 )
	{
		this -> true_char = next_param;
		this -> genNullChar();
	}
	else if( strcmp( param, "--falsechar" ) == 0 )
	{
		this -> false_char = next_param;
		this -> genNullChar();
	}
	else
	{
		cout << "Unknow parametr " << param << "\n\n";
		this -> help();
	}
}

/**
 * Method to count number of chars in string. There is problem in pure c++ to count utf-8 char as one char.
 *
 * @param string str - String content to count.
 * @return int
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
int binaryClock::utf8_strlen( const string& str )
{
	int size = 0;
	for( int i = 0; i < str.length(); i++, size++)
	{
		int charnr = (unsigned char) str[i];
		if(( charnr >= 0 ) && ( charnr <= 127 ))
			i += 0;
		else if(( charnr & 0xE0 ) == 0xC0 )
			i++;
		else if(( charnr & 0xF0 ) == 0xE0 )
			i += 2;
		else if(( charnr & 0xF8 ) == 0xF0 )
			i += 3;
		else if(( charnr & 0xFC ) == 0xF8 )
			i += 4;
		else if(( charnr & 0xFE ) == 0xFC )
			i += 5;
		else
			return 0;
	}
	return size;
}

/**
 * Main function of program.
 *
 * @version 1.0 [19.08.2015]
 * @author krzmig
 */
int main( int argc, char* argv[] )
{
	binaryClock* bc = new binaryClock;
	for( int i = 1; i < argc; i++ )
	{
		if( i + 1 < argc )
			bc -> checkParam( argv[i], argv[i+1] );
		else
			bc -> checkParam( argv[i], (char *)"" );
	}
	bc -> run();
	delete bc;
}

