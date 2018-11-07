#include <Rcpp.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <exception>
#include "PajeUnity.h"
bool yywrap = false;

using namespace Rcpp;

//Parameters
bool flex = false;
bool strict = true;
double stopat = -1;
static bool ignoreIncompleteLinks = true;
char *probabilistic = NULL;
bool noImbrication = false;
bool userDefined = false;
bool outofcore = false;
bool entityDump = false;
bool quiet = false;

List _dump (PajeComponent *simulator)
{
  struct {
    CharacterVector nature;
    CharacterVector container;
    CharacterVector type;
    DoubleVector start;
    DoubleVector end;
    DoubleVector duration;
    IntegerVector imbrication;
    CharacterVector value;
  } st;

  struct {
    CharacterVector nature;
    CharacterVector container;
    CharacterVector type;
    DoubleVector start;
    DoubleVector end;
    DoubleVector duration;
    DoubleVector value;
  } va;

  struct {
    CharacterVector nature;
    CharacterVector container;
    CharacterVector type;
    DoubleVector start;
    CharacterVector value;
  } ev;

  struct {
    CharacterVector nature;
    CharacterVector container;
    CharacterVector type;
    DoubleVector start;
    DoubleVector end;
    DoubleVector duration;
    CharacterVector value;
    CharacterVector startContainer;
    CharacterVector endContainer;
    CharacterVector key;
  } li;
  
  double si = simulator->startTime();
  double ei = simulator->endTime();

  std::vector<PajeContainer*> stack;
  stack.push_back (simulator->rootInstance());

  while (!stack.empty()){
    PajeContainer *c = stack.back();
    stack.pop_back ();

    //recurse on all types of children
    std::vector<PajeType*> containedTypes;
    containedTypes = simulator->containedTypesForContainerType (c->type());

    for (PajeType *t : containedTypes){
      if (t->nature() == PAJE_ContainerType){
	std::vector<PajeContainer*> children;
	children = simulator->enumeratorOfContainersTypedInContainer (t, c);
	for (PajeContainer *child : children){
	  stack.push_back (child);
	}
      }else{
	std::vector<PajeEntity*> entities;
	entities = simulator->enumeratorOfEntitiesTypedInContainer (t, c, si, ei);
	for(PajeEntity *e : entities){
	  if (t->nature() == PAJE_StateType){
	    st.nature.push_back("State");
	    st.container.push_back(e->container()? e->container()->name() : "NULL");
	    st.type.push_back(e->type()? e->type()->name() : "NULL");
	    st.start.push_back(e->startTime());
	    st.end.push_back(e->endTime());
	    st.duration.push_back(e->duration());
	    st.imbrication.push_back(e->imbricationLevel());
	    st.value.push_back(e->value()? e->value()->name() : "NULL");
	  }else if(t->nature() == PAJE_VariableType){
	    va.nature.push_back("Variable");
	    va.container.push_back(e->container()? e->container()->name() : "NULL");
	    va.type.push_back(e->type()? e->type()->name() : "NULL");
	    va.start.push_back(e->startTime());
	    va.end.push_back(e->endTime());
	    va.duration.push_back(e->duration());
	    va.value.push_back(e->doubleValue());
	  }else if(t->nature() == PAJE_EventType){
	    ev.nature.push_back("Variable");
	    ev.container.push_back(e->container()? e->container()->name() : "NULL");
	    ev.type.push_back(e->type()? e->type()->name() : "NULL");
	    ev.start.push_back(e->startTime());
	    ev.value.push_back(e->value()? e->value()->name() : "NULL");
	  }else if(t->nature() == PAJE_LinkType){
	    li.nature.push_back("State");
	    li.container.push_back(e->container()? e->container()->name() : "NULL");
	    li.type.push_back(e->type()? e->type()->name() : "NULL");
	    li.start.push_back(e->startTime());
	    li.end.push_back(e->endTime());
	    li.duration.push_back(e->duration());
	    li.value.push_back(e->value()? e->value()->name() : "NULL");
	    li.startContainer.push_back(e->startContainer()? e->startContainer()->name() : "NULL");
	    li.endContainer.push_back(e->endContainer()? e->endContainer()->name() : "NULL");
	    li.key.push_back(e->key());
	  }
	}
      }
    }
  }
  DataFrame stDF = DataFrame::create(Named("Nature") = st.nature,
				     Named("Container") = st.container,
				     Named("Type") = st.type,
				     Named("Start") = st.start,
				     Named("End") = st.end,
				     Named("Duration") = st.duration,
				     Named("Imbrication") = st.imbrication,
				     Named("Value") = st.value);
  DataFrame vaDF = DataFrame::create(Named("Nature") = va.nature,
				     Named("Container") = va.container,
				     Named("Type") = va.type,
				     Named("Start") = va.start,
				     Named("End") = va.end,
				     Named("Duration") = va.duration,
				     Named("Value") = va.value);
  DataFrame evDF = DataFrame::create(Named("Nature") = ev.nature,
				     Named("Container") = ev.container,
				     Named("Type") = ev.type,
				     Named("Start") = ev.start,
				     Named("Value") = ev.value);
  DataFrame liDF = DataFrame::create(Named("Nature") = li.nature,
				     Named("Container") = li.container,
				     Named("Type") = li.type,
				     Named("Start") = li.start,
				     Named("End") = li.end,
				     Named("Duration") = li.duration,
				     Named("Value") = li.value,
				     Named("StartContainer") = li.startContainer,
				     Named("EndContainer") = li.endContainer,
				     Named("Key") = li.key);
  return List::create(Named("state") = stDF,
		      Named("variable") = vaDF,
		      Named("event") = evDF,
		      Named("link") = liDF);
}

// [[Rcpp::export]]
List pajeng_read (std::string input) {
    
  PajeUnity *unity = new PajeUnity(flex,
				   strict,
				   input,
				   stopat,
				   ignoreIncompleteLinks,
				   probabilistic,
				   noImbrication,
				   userDefined,
				   outofcore,
				   entityDump,
				   quiet);

    List z = _dump(unity);
    delete unity;
    return z;
}

