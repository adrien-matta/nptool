#include "MugastMap.h"



////////////////////////////////////////////////////////////////////////////////
void Check(const int* array){
  // Test Trap X
  set<unsigned int> Strip;
  unsigned int count=0;
  bool checkStrip = true;
  for(unsigned int i = 0 ; i < 128 ; i++){
    // Check that the value does not exist
    if(Strip.find(array[i])==Strip.end())
      Strip.insert(array[i]);
    else{
      cout << "Strip duplicate value : " << array[i] << " count:" << count++ <<  endl; 
      checkStrip=false;
      } 
    if(array[i]>128 || array[i]<1){
      cout << "Strip value " << array[i] << " out of bound" << endl;
      checkStrip=false;
      }
  }
  if(checkStrip&&Strip.size()==128)
    cout << "Strip : No error found " << endl;
  else
    cout << "Strip : Error, size: " << Strip.size() << endl;
}
////////////////////////////////////////////////////////////////////////////////
void test(){
  
  cout << "// Test TrapX //" << endl; 
  Check(MUGAST_MAP::TrapezeX);
  cout << "// Test TrapY //" << endl; 
  Check(MUGAST_MAP::TrapezeY);
  cout << "// Test SquareX //" << endl; 
  Check(MUGAST_MAP::SquareX);
  cout << "// Test SquareY //" << endl; 
  Check(MUGAST_MAP::SquareY);
  cout << "// Test AnnularX //" << endl; 
  Check(MUGAST_MAP::AnnularX);
  cout << "// Test AnnularY //" << endl; 
  Check(MUGAST_MAP::AnnularY);
  



  }


