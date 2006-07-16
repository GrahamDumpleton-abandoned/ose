#ifndef MKSRC_HH
#define MKSRC_HH
/*
// ============================================================================
//
// = FILENAME
//     mksrc.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>

/* ------------------------------------------------------------------------- */

class APP_ConfigItem
{
    friend istream&	operator>>(istream& ins, APP_ConfigItem& theItem);

  public:

			APP_ConfigItem()
				{}

			APP_ConfigItem(APP_ConfigItem const& theItem);

    APP_ConfigItem&	operator=(APP_ConfigItem const& theItem);

    static OTC_String const&	file()
				{ return file_; }

    static void		setFile(OTC_String const& theFile)
				{ file_ = theFile; }

    OTC_String const&	project() const
				{ return project_; }

    OTC_String const&	pattern() const
				{ return pattern_; }

    OTC_String const&	program() const
				{ return program_; }

    OTC_String const&	helpFile() const
				{ return helpFile_; }

  private:

    static OTC_String	file_;

    OTC_String		project_;

    OTC_String		pattern_;

    OTC_String		program_;

    OTC_String		helpFile_;
};


/* ------------------------------------------------------------------------- */

#endif /* MKSRC_HH */
