#include <Rcpp.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <exception>
#include "PajeUnity.h"

using namespace Rcpp;

bool yywrap = false;

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

DataFrame _dump (PajeComponent *simulator)
{
  CharacterVector nature;
  CharacterVector container;
  CharacterVector type;
  DoubleVector start;
  DoubleVector end;
  DoubleVector duration;
  IntegerVector imbrication;
  CharacterVector value;
  
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
	    nature.push_back("State");
	    container.push_back(e->container()? e->container()->name() : "NULL");
	    type.push_back(e->type()? e->type()->name() : "NULL");
	    start.push_back(e->startTime());
	    end.push_back(e->endTime());
	    duration.push_back(e->duration());
	    imbrication.push_back(e->imbricationLevel());
	    value.push_back(e->value()? e->value()->name() : "NULL");
	  }else if(t->nature() == PAJE_VariableType){
	  }else if(t->nature() == PAJE_EventType){
	  }else if(t->nature() == PAJE_LinkType){
	  }
	}
      }
    }
  }

  DataFrame ret = DataFrame::create(Named("Nature") = nature,
				    Named("Container") = container,
				    Named("Type") = type,
				    Named("Start") = start,
				    Named("End") = end,
				    Named("Duration") = duration,
				    Named("Imbrication") = imbrication,
				    Named("Value") = value);

  // CharacterVector n;
  // n.push_back("Nature");
  // n.push_back("Container");
  // n.push_back("Type");
  // n.push_back("Start");
  // n.push_back("End");
  // n.push_back("Duration");
  // n.push_back("Imbrication");
  // n.push_back("Value");

  // ret.attr("names") = n;
  return ret;
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

    CharacterVector x = CharacterVector::create( "foo", "bar" )  ;
    NumericVector y   = NumericVector::create( 0.0, 1.0 ) ;

    DataFrame z = _dump(unity);
    delete unity;
    return List::create(z);
}

