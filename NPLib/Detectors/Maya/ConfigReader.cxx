/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Vandebrouck   contact address: vandebro@ipno.in2p3.fr *
 *                  J. Gibelin                      gibelin@lpccaen.in2p3.fr *
 *                                                                           *
 * Creation Date  : November 2011                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds a the configuration reader part for Maya     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include <sstream>
#include "ConfigReader.h"

using std::cout;
using std::endl;
using std::cerr;
using std::vector;

//ClassImp(ConfigReader)

ConfigReader::ConfigReader(){
  // ForceInstantiation() ;
  number_of_files = -1 ;
  files_already_read = false ;
  list_of_files_.clear();
}

ConfigReader::ConfigReader( std::vector<string> &v ){
  //  ForceInstantiation() ;
  files_already_read = false ;
  list_of_files_.assign(v.begin(),v.end());
  number_of_files = v.size() ;
}

ConfigReader::ConfigReader( char *f1, ...){
  //  ForceInstantiation() ;
  va_list marker;
  char *file ;
  list_of_files_.clear();
  files_already_read = false ;

  number_of_files = 1 ;    
  va_start( marker, f1 );     /* Initialize variable arguments. */


  trim(f1) ;

  list_of_files_.push_back(f1) ;

  while((file = va_arg( marker, char*)) != NULL)
    {

      trim(file) ;

      if(file !=NULL && strlen(file)>0){

	list_of_files_.push_back(file);
	number_of_files++ ;

      }

    }
  va_end( marker );              /* Reset variable arguments.      */



}


void ConfigReader::__ForceInstantiation__(){
  bool b ;
  Read<bool>("",b);

  //
  std::vector<int> iv1 ;
  std::vector<std::vector<int> > iv2 ;
  int i ;
  Read<int>("",iv1,-1);
  Read<int>("",iv2);
  Read<int>("",i);

  //
  std::vector<float> fv1 ;
  std::vector<std::vector<float> > fv2 ;
  float f ;
  Read<float>("",fv1,-1);
  Read<float>("",fv2);
  Read<float>("",f);

  //
  std::vector<double> dv1 ;
  std::vector<std::vector<double> > dv2 ;
  double d ;
  Read<double>("",dv1,-1);
  Read<double>("",dv2);
  Read<double>("",d);

  

}


int ConfigReader::AddFile(char *f){
  if(f != NULL) list_of_files_.push_back(f) ;
  number_of_files = list_of_files_.size() ;
  return number_of_files ;
}

template<typename T> void ConfigReader::PrintVector( std::vector<T> & v1){
  if(v1.empty()) {
    cout << "1D Vector empty" << endl ;
    return ;
  }

  for(unsigned int i=0;i<v1.size();i++){
    cout << v1[i] << ", " ;
  }
  cout << "" <<endl ;
}

template<typename T> void ConfigReader::PrintVector( std::vector<std::vector<T> >& v2){
  if(v2.empty()) {
    cout << "2D Vector empty" << endl ;
    return ;
  }
  for(unsigned int i=0;i<v2.size();i++){
    PrintVector(v2[i]);
  }
  cout << endl ;
}

void ConfigReader::trim( char *c ){
  string s(c);
  trim(s);
  c = (char*)s.c_str();
}

void ConfigReader::trim( string& s )
{
        // Remove leading and trailing whitespace
        static const char whitespace[] = " \n\t\v\r\f";
        s.erase( 0, s.find_first_not_of(whitespace) );
        s.erase( s.find_last_not_of(whitespace) + 1U );
}

void ConfigReader::clean( string& s )
{
  //cout << "avant :" << s << endl ;
  // Trim string
  trim(s) ;
  // Uncomment string
  uncomment(s) ; 
  // Remove all possible delimiters other than space
  static const string delimiters = ",\n\t\v\r\f";
  unsigned int position = s.find_first_of( delimiters );
  while ( position != (unsigned int)string::npos ) 
    {
      s.replace( position, 1, " ");
      position = s.find_first_of( delimiters, position + 1 );
    } 
  //cout << "apres :" << s << endl ;
}

void ConfigReader::uncomment( string& s )
{
  // Remove trailing comments
   static const string comments = "//" ;
  if(s.find(comments) < s.length()){
    s.erase( s.find(comments) );
  }

}


int ConfigReader::FindTokenInFile(string filename){
  std::ifstream parfile;
  string line,nextline,token ;

  parfile.open(filename.c_str());

  if (!parfile) {
    std::cerr << "Unable to open file " << filename << endl;
    return -1 ;
  }

  line = "" ;
  nextline = "" ;
  while(parfile)
    {
      // Read an entire line at a time
      if(line.length()==0 && nextline.length()==0) {
	std::getline( parfile, line );
      }
      trim(line) ;
      if(!line.compare(0,2,"//") || line.compare(0,1,"[")) {
	line = "" ; nextline = "" ;
	continue ;
      }
      if(!line.compare(0,1,"[")) {
	token = line.substr(1,line.find("]")-1);
	trim(token);
	cout << " token \e[32m'"  << token << "'\e[0m  found "<<endl ;

	if((parameters_blocks.count(token)) == 0){
	  parameters_blocks[token] =  new std::stringstream(std::stringstream::in | std::stringstream::out) ;
	  parameters_nlines[token] = 0 ;
	  parameters_written_in[token] = filename ;
	  parameters_nparam[token] = 0  ;
	  while(parfile){
	    std::getline( parfile, nextline );
	    clean(nextline) ;
	    if(!nextline.compare(0,1,"[")) break ;
	    else if(nextline.length()==0) continue ;
	    *parameters_blocks[token] << "\n" ;
	    *parameters_blocks[token] <<  nextline ;
	    parameters_nlines[token]++ ;
	    // -- Count the number of parameters --
	    std::stringstream ss(nextline); // Insert the string into a stream
	    std::string buf ;
	    while (ss >> buf){
	      parameters_nparam[token]++;
	    }

	    // --------
	  }
	  line = nextline ;
	  nextline = "" ;
	  //	  cout << *parameters_blocks[token] << endl ;
	} else {
	  cerr << "\nWarning: token \e[31m'"  << token 
	       << "'\e[0m in  \e[31m" << filename 
	       << "\e[0m already exists in \e[31m" << parameters_written_in[token] <<"\e[0m. Skip.\n "<<endl ;
	  line = "" ; nextline = "" ;
	}
      }
    }

  parfile.close();
  files_already_read = true ;

  return files_already_read ;

}

int ConfigReader::FindAllToken(void){
  tot_number_of_token = 0 ;
  if(number_of_files>0){
    for(int i=0;i<number_of_files;i++){      
      cout << "Now reading " << list_of_files_.at(i) << endl ;
      tot_number_of_token += FindTokenInFile(list_of_files_.at(i))  ;
    }
    return tot_number_of_token ;
  }
  return -1 ;
}

int ConfigReader::PrintListofToken(void){
  if(PrintListofFiles()>0){
    for(int i=0;i<(int)list_of_files_.size();i++){
      cout << "In file: " << list_of_files_.at(i) << endl << endl ;
      
      for ( std::map< string,std::stringstream*  >::const_iterator iter = parameters_blocks.begin();
	    iter != parameters_blocks.end(); ++iter ){
	if(parameters_written_in[iter->first].compare(list_of_files_.at(i))==0){
	  ((std::stringstream*)iter->second)->seekg(0,std::ios::beg);
	  cout << "\e[33m***** " << iter->first << " ******\e[0m\n" 
	       << ((std::stringstream*)iter->second)->rdbuf() << '\n' << endl;
	  cout << "\e[34m### Stats: " << parameters_nlines[iter->first]<< " lines ### "   ;
	  cout << parameters_nparam[iter->first] << " params ###\e[0m\n" << endl ;
	}
      }
      
    }
    return (int)list_of_files_.size();
  }
  return 0 ;
}

int ConfigReader::PrintListofFiles(void){
  if(number_of_files>0){
    cout << number_of_files << " parameter file(s) defined :" << endl ;
    for(int i=0;i<(int)list_of_files_.size();i++){      
      cout << "  " << i+1 << ") " << list_of_files_.at(i) << " " << endl  ;
    }
    cout << endl ;
    return number_of_files ;
  } else {
    cout << "No parameter files are defined" << endl << endl ;
    return -1 ;
  }
}

template<typename T> int ConfigReader::Read(string token, std::vector<std::vector <T> >&v2){
  if(token.size()==0) return 0;
  if(!files_already_read) {
    if (FindAllToken() == -1) {
      for(unsigned int i=0;i<v2.size();i++){
	v2.at(i).clear() ; // clear the vector.
      }
      v2.clear();
      return 0 ;
    }
  }

  if((parameters_nparam).count(token)==0){
    cout << "Warning: token \e[31m'" << token << "'\e[0m not found  in any files " << endl ;
    for(unsigned int i=0;i<v2.size();i++){
      v2.at(i).clear() ; // clear the vector.
    }
    v2.clear();
    return 0;
  }

  // cout << "vec^2 of " << typeid(T).name() << " and size " << v2.size() << " " << v2.at(0).size() << endl ;
  unsigned int npar = 0 ;
  for(unsigned int i=0;i<v2.size();i++){
    npar += Read(token,v2.at(i),2);
  }

  if(v2.size()*v2.at(0).size() != npar){ // mismatch between the number of parameters read and expected.
    cerr << "\nWarning: 2D vector used to fill in parameters from token \e[31m'" << token 
	 << "'\e[0m has a dimension of  \e[31m" <<  v2.size() << "x" << (v2.at(0).size()) << "=" 
	 << (v2.size()*(v2.at(0).size())) << "\e[0m" << endl ;
    cerr << "\tbut \e[31m" << parameters_nparam[token] << "\e[0m parameters were read in \e[31m" 
	 <<  parameters_written_in[token] << "\e[0m" << endl ;
    for(unsigned int i=0;i<v2.size();i++){
      v2.at(i).clear() ; // clear the vector.
    }
    v2.clear();
    return 0 ;
  }
  //  PrintVector(v2) ;
  return npar ;
}

template<typename T> int ConfigReader::Read(string token, std::vector<T> &v1, int dimension){
  if((dimension<1) || (token.size()==0) ) return 0 ;
  if(!files_already_read) {
    if (FindAllToken() == -1) {
      v1.clear() ; // if no file is found the vector is cleared ! 
      return 0 ;
    }
  }

  if((parameters_nparam).count(token)==0){
    cout << "Warning: token \e[31m'" << token << "'\e[0m not found  in any files " << endl ;
    v1.clear();
    return 0;
  }


  if(dimension==1){ // check if the function as been call directly to fill a 1D vector (default)
    parameters_blocks[token]->seekg(0,std::ios::beg); // rewind only if 1D 
    if((int)v1.size() != parameters_nparam[token]){ // size mismatch
      cerr << "\nWarning: 1D vector used to fill in parameters from token \e[31m'" << token 
	   << "'\e[0m has a dimension of  \e[31m" <<  v1.size() << "\e[0m" << endl ;
      cerr << "\tbut \e[31m" << parameters_nparam[token] << "\e[0m parameters were read in \e[31m" 
	   <<  parameters_written_in[token] << "\e[0m" << endl ;
      v1.clear();
      return 0 ;
    }
  }

  //  cout << "vec^1 of " << typeid(T).name() << ", size " << v1.size() << endl ;
  // Tokenize
  T buf ;
  unsigned int npar = 0 ;

  while ((*parameters_blocks[token] >> buf)){
    v1.at(npar++) =  static_cast<T>(buf);
    if(npar>=v1.size()) break ;
  }
  return npar ;
}

template<typename T> int ConfigReader::Read(string token, T &a){
  if(token.size()==0)  return 0 ;
  
  if(!files_already_read) {
    if (FindAllToken() == -1) {
      a = 0 ; // if no file is found the coefficinnt is cleared
      return 0 ;
    }
  }
  
  if((parameters_nparam).count(token)==0){
    cout << "Warning: token \e[31m'" << token << "'\e[0m not found  in any files " << endl ;
    a = 0 ;
    return 0;
  }
  
  *parameters_blocks[token] >> a ;

  return 1 ;
}

/*
int main(void) {

  ConfigReader *cal_par = new ConfigReader();

    cout << cal_par->AddFile("./test1.prm") << endl ;
    cout << cal_par->AddFile("./test2.prm") << endl ;
  
  //  ConfigReader *cal_par = new ConfigReader("./test1.prm","./test2.prm");

  //  cal_par->FindAllToken() ;
  std::vector<std::vector <int> > matrice(3,vector<int>(4,0)) ;
  cal_par->Read("par01",matrice);
  
  std::vector<float>  vecteur(7,0) ;
  cal_par->Read("par2",vecteur);
  
  cal_par->PrintVector(matrice);  
  cal_par->PrintVector(vecteur);  
  
  //  cout << cal_par->GetListofFiles().at(0) << endl ;

  //cal_par->PrintListofToken();
  delete cal_par ;
  return 0 ;
}
*/

