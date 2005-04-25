#ifndef TSTITEM1_HH
#define TSTITEM1_HH

#include <OTC/debug/tracer.hh>

class TST_Item1
{
  public:

			TST_Item1(u_int theValue);

			~TST_Item1();

    int			value() const
				{ return value_; }

  private:

			TST_Item1(TST_Item1 const& theItem);

    TST_Item1&		operator=(TST_Item1 const& theItem);

    int			value_;
};

inline TST_Item1::TST_Item1(u_int theValue)
  : value_(theValue)
{
  OTC_Tracer tracer("TST_Item1::TST_Item1()");
  tracer() << "this->value() = " << value() << endl;
}

inline TST_Item1::~TST_Item1()
{
  OTC_Tracer tracer("TST_Item1::~TST_Item1()");
  tracer() << "this->value() = " << value() << endl;
}

#endif /* TSTITEM1_HH */
