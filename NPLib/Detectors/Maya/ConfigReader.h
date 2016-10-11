#ifndef __CREADER__
#define __CREADER__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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



#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <cstdarg>
#include <cstdio>
#include <cstring>

using std::string;

class ConfigReader{
  // Data
 protected:
  std::map<string,std::stringstream*> parameters_blocks ; // entire data black following the presence of a token
  std::map<string,string> parameters_written_in ; // file name where a given token is written in
  std::map<string,int> parameters_nlines ; // number of parameters line for a given token 
  std::map<string,int> parameters_nparam ; // total number of parameters for a gien token
  std::vector<string> list_of_files_ ; // list of all the parameter files given
  int number_of_files ; // number of parameter files
  int tot_number_of_token ; // total number of token
  std::vector<int> number_of_token_per_file ; // number of token per parameter file
  bool files_already_read ;


  // Methods
 public:
  ConfigReader( char*, ...);
  ConfigReader( std::vector<string> &);
  ConfigReader();
  ~ConfigReader(){ 
    std::cout <<  "ConfigReader: Clean the mess" << std::endl ; 

    parameters_written_in.clear();

    parameters_blocks.clear();
    //map<string,std::stringstream*>::iterator iter;   
    //for( iter = parameters_blocks.begin(); iter != parameters_blocks.end(); iter++ ) {
    //
    //  ((std::stringstream*)iter->second) ==;
    //}

    parameters_nlines.clear(); 
    parameters_nparam.clear(); 
    list_of_files_.clear(); 
    number_of_token_per_file.clear();
  };

  //void Read(const string& , const std::vector<std::vector<int> >& ) const ; // call as read<T>
  int AddFile(char*);
  template<typename T> int Read(string, std::vector<std::vector<T> >&) ;
  template<typename T> int Read(string, std::vector<T> &, int) ;
  template<typename T> int Read(string, T &) ;

  int FindAllToken() ;
  int FindTokenInFile(string) ;
  
  int Dump(void){ return PrintListofToken() ;};
  int PrintListofFiles(void);
  int PrintListofToken(void);

  std::vector<string> &GetListofFiles() {return list_of_files_ ; } ;

  template<typename T> void PrintVector( std::vector<T> &) ; // not directly related but very usefull !!
  template<typename T> void PrintVector( std::vector<std::vector<T> > &) ; // not directly related but very usefull !!

 private:

  void __ForceInstantiation__() ;
  void trim( string&) ;
  void trim( char *) ;
  void clean(string&);
  void uncomment( string&) ;

  //  ClassDef(ConfigReader,1) 

};

#endif

