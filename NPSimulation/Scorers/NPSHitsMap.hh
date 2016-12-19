/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : December 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Hits map clone from geant4 10.2 to avoid incompatibility with 10.3       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *****************************************************************************/
#ifndef HitsMap_h
#define HitsMap_h 1

#include "G4THitsCollection.hh"
#include "globals.hh"
#include <map>

// class description:
//
//  This is a template class of hits map and parametrized by
// The concrete class of G4VHit. This is a uniform collection for
// a particular concrete hit class objects.
//  An intermediate layer class G4HitsMap appeared in this
// header file is used just for G4Allocator, because G4Allocator
// cannot be instansiated with a template class. Thus G4HitsMap
// class MUST NOT be directly used by the user.
namespace NPS{
template <typename T> class HitsMap : public G4HitsCollection 
{
  public:
      HitsMap();
  public: // with description
      HitsMap(G4String detName,G4String colNam);
      // constructor.
  public:
      virtual ~HitsMap();
      G4int operator==(const HitsMap<T> &right) const;
      HitsMap<T> & operator+=(const HitsMap<T> &right) const;

  public: // with description
      virtual void DrawAllHits();
      virtual void PrintAllHits();
      //  These two methods invokes Draw() and Print() methods of all of
      // hit objects stored in this map, respectively.

  public: // with description
      inline T* operator[](G4int key) const;

      //  Returns a pointer to a concrete hit object.
      inline std::map<G4int,T*>* GetMap() const
      { return (std::map<G4int,T*>*)theCollection; }
      //  Returns a collection map.
      inline G4int add(const G4int & key, T * &aHit) const;
      inline G4int add(const G4int & key, T &aHit) const;
      //  Insert a hit object. Total number of hit objects stored in this
      // map is returned.
      inline G4int set(const G4int & key, T * &aHit) const;
      inline G4int set(const G4int & key, T &aHit) const;
      //  Overwrite a hit object. Total number of hit objects stored in this
      // map is returned.
      inline G4int entries() const
      { return ((std::map<G4int,T*>*)theCollection)->size(); }
      //  Returns the number of hit objects stored in this map
      inline void clear();

  public:
    virtual G4VHit* GetHit(size_t) const {return 0;}
    virtual size_t GetSize() const
    { return ((std::map<G4int,T*>*)theCollection)->size(); }

};

template <typename T> HitsMap<T>::HitsMap()
{ 
  theCollection = (void*)new std::map<G4int,T*>;
}

template <typename T> HitsMap<T>::HitsMap(G4String detName,G4String colNam)
    : G4HitsCollection(detName,colNam)
{ 
    theCollection = (void*)new std::map<G4int,T*>;
}

template <typename T> HitsMap<T>::~HitsMap()
{
  typename std::map<G4int,T*> * theHitsMap = GetMap();
  typename std::map<G4int,T*>::iterator itr = theHitsMap->begin();
  for(; itr != theHitsMap->end(); itr++) {
      delete itr->second;
  }

  delete theHitsMap;
}

template <typename T> G4int HitsMap<T>::operator==(const HitsMap<T> &right) const
{ return (collectionName==right.collectionName); }

template <typename T> HitsMap<T> &
HitsMap<T>::operator+=(const HitsMap<T> &right) const
{
    std::map<G4int,T*> * aHitsMap = right.GetMap();
    typename std::map<G4int,T*>::iterator itr = aHitsMap->begin();
    for(; itr != aHitsMap->end(); itr++) {
	add(itr->first, *(itr->second));
    }
    return (HitsMap<T>&)(*this);
}

template <typename T> inline T* 
HitsMap<T>::operator[](G4int key) const {
    std::map<G4int,T*> * theHitsMap = GetMap();
    if(theHitsMap->find(key) != theHitsMap->end()) {
	return theHitsMap->find(key)->second;
    } else {
	return 0;
    }
}

template <typename T> inline G4int
HitsMap<T>::add(const G4int & key, T * &aHit) const {

    typename std::map<G4int,T*> * theHitsMap = GetMap();
    if(theHitsMap->find(key) != theHitsMap->end()) {
	*(*theHitsMap)[key] += *aHit;
    } else {
	(*theHitsMap)[key] = aHit;
    }
    return theHitsMap->size();
}

template <typename T> inline G4int
HitsMap<T>::add(const G4int & key, T &aHit) const {

    typename std::map<G4int,T*> * theHitsMap = GetMap();
    if(theHitsMap->find(key) != theHitsMap->end()) {
	*(*theHitsMap)[key] += aHit;
    } else {
	T * hit = new T;
	*hit = aHit;
	(*theHitsMap)[key] = hit;
    }

    return theHitsMap->size();
}

template <typename T> inline G4int
HitsMap<T>::set(const G4int & key, T * &aHit) const {
                                                                                             
    typename std::map<G4int,T*> * theHitsMap = GetMap();
    if(theHitsMap->find(key) != theHitsMap->end()) {
        delete (*theHitsMap)[key]->second;
    }
    (*theHitsMap)[key] = aHit;
    return theHitsMap->size();
}
                                                                                             
template <typename T> inline G4int
HitsMap<T>::set(const G4int & key, T &aHit) const {
                                                                                             
    typename std::map<G4int,T*> * theHitsMap = GetMap();
    if(theHitsMap->find(key) != theHitsMap->end()) {
        *(*theHitsMap)[key] = aHit;
    } else {
        T * hit = new T;
        *hit = aHit;
        (*theHitsMap)[key] = hit;
    }
                                                                                             
    return theHitsMap->size();
}
                                                                                             
template <typename T> void HitsMap<T>::DrawAllHits() 
{;}

template <typename T> void HitsMap<T>::PrintAllHits() 
{
 G4cout << "HitsMap " << SDname << " / " << collectionName << " --- " << entries() << " entries" << G4endl;
/*----- commented out for the use-case where <T> cannot be initialized
        to be zero or does not support += operator.
 std::map<G4int,T*> * theHitsMap = GetMap();
 typename std::map<G4int, T*>::iterator itr = theHitsMap->begin();
 T sum = 0.;
 for(; itr != theHitsMap->end(); itr++) {
  ///////////////////////////////G4cout << "  " << itr->first << " : " << *(itr->second) << G4endl;
  sum += *(itr->second);
 }
 G4cout << "             Total : " << sum << G4endl;
----------------------------------------------------------------------*/
}

template <typename T> void HitsMap<T>::clear() {

    std::map<G4int,T*> * theHitsMap = GetMap();
    typename std::map<G4int, T*>::iterator itr = theHitsMap->begin();
    for(; itr != theHitsMap->end(); itr++) {
	delete itr->second;
    }
    theHitsMap->clear();

}
}
#endif

