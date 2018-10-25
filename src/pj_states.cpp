#include <Rcpp.h>
#include "PajeUnity.h"

using namespace Rcpp;

static CharacterVector nature;
static CharacterVector container;
static CharacterVector type;
static DoubleVector start;
static DoubleVector end;
static DoubleVector duration;
static IntegerVector imbrication;
static CharacterVector value;

void _add_state_entity (PajeEntity *e)
{
  nature.push_back("State");
  container.push_back(e->container()? e->container()->name() : "NULL");
  type.push_back(e->type()? e->type()->name() : "NULL");
  start.push_back(e->startTime());
  end.push_back(e->endTime());
  duration.push_back(e->duration());
  imbrication.push_back(e->imbricationLevel());
  value.push_back(e->value()? e->value()->name() : "NULL");
}

DataFrame _collect_state_dataframe (void)
{
  DataFrame ret = DataFrame::create(Named("Nature") = nature,
				    Named("Container") = container,
				    Named("Type") = type,
				    Named("Start") = start,
				    Named("End") = end,
				    Named("Duration") = duration,
				    Named("Imbrication") = imbrication,
				    Named("Value") = value);
  return ret;
}
