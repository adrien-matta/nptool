#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Rayin.h"
using namespace std;


Rayin::Rayin(const string& filename, bool check):
  isOwner(true)
{
  if(check)
    {
      int length;
      {
	ifstream test_("rayin.dat");
	test_.seekg(0, ios::end);
	length = test_.tellg();
      }
      if(length > 0) {
	string answer;
	cerr << "\nWARNING: The file \"rayin.dat\" already exists in the current directory. "
	     << "Enter 'y' to remove it and replace it with a link to \"" << filename << "\" "
	     << "OR enter 'n' to continue with the existing \"rayin.dat\" file.\n"
	     << "Or enter 'q' to abort the program...\n";
	while(1) {
	  cin >> answer;
	  if     (answer == "y" || answer == "Y") { break;                  }
	  else if(answer == "n" || answer == "N") { isOwner = false; break; }
	  else if(answer == "q" || answer == "Q") { exit(1);                }
	  else {
	    cerr << "ERROR: invalid response: \"" << answer << "\". Please enter 'y', 'n', or 'q'\n";
	  }
	}
      }
    }
  if(isOwner) { 
    cerr << "Creating link \"rayin.dat\" to the file \"" << filename << "\"...\n";
    stringstream sstr;
    sstr << "ln -fs " <<filename << " rayin.dat";
    system(sstr.str().c_str());
  }
}

Rayin::~Rayin()
{
  if(isOwner) {
    cerr << "Removing link \"rayin.dat\"...\n";
    system("rm -f rayin.dat");
  }
}
