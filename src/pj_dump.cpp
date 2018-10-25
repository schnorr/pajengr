#include <Rcpp.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <exception>
#include "PajeUnity.h"
bool yywrap = false;

using namespace Rcpp;

void _add_state_entity (PajeEntity *e);
void _add_variable_entity (PajeEntity *e);
void _add_event_entity (PajeEntity *e);
void _add_link_entity (PajeEntity *e);
DataFrame _collect_state_dataframe (void);
DataFrame _collect_variable_dataframe (void);
DataFrame _collect_event_dataframe (void);
DataFrame _collect_link_dataframe (void);

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
	    _add_state_entity (e);
	  }else if(t->nature() == PAJE_VariableType){
	    _add_variable_entity (e);
	  }else if(t->nature() == PAJE_EventType){
	    _add_event_entity (e);
	  }else if(t->nature() == PAJE_LinkType){
	    _add_link_entity (e);
	  }
	}
      }
    }
  }

  return List::create(_collect_state_dataframe(),
		      _collect_variable_dataframe(),
		      _collect_event_dataframe(),
		      _collect_link_dataframe());
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

    List z = _dump(unity);
    delete unity;
    return z;
}

