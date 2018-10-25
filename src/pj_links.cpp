#include <Rcpp.h>
#include "PajeUnity.h"

using namespace Rcpp;

static CharacterVector nature;
static CharacterVector container;
static CharacterVector type;
static DoubleVector start;
static DoubleVector end;
static DoubleVector duration;
static CharacterVector value;
static CharacterVector startContainer;
static CharacterVector endContainer;
static CharacterVector key;

void _add_link_entity (PajeEntity *e)
{
  nature.push_back("State");
  container.push_back(e->container()? e->container()->name() : "NULL");
  type.push_back(e->type()? e->type()->name() : "NULL");
  start.push_back(e->startTime());
  end.push_back(e->endTime());
  duration.push_back(e->duration());
  value.push_back(e->value()? e->value()->name() : "NULL");
  startContainer.push_back(e->startContainer()? e->startContainer()->name() : "NULL");
  endContainer.push_back(e->endContainer()? e->endContainer()->name() : "NULL");
  key.push_back(e->key());
}

DataFrame _collect_link_dataframe (void)
{
  return DataFrame::create(Named("Nature") = nature,
			   Named("Container") = container,
			   Named("Type") = type,
			   Named("Start") = start,
			   Named("End") = end,
			   Named("Duration") = duration,
			   Named("Value") = value,
			   Named("StartContainer") = startContainer,
			   Named("EndContainer") = endContainer,
			   Named("Key") = key);
}
