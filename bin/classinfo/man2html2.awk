#!/bin/nawk

{
  if ( initialised == 0 )
  {
    num = 0
    if ( link != "" )
    {
      while ( "ls " link " 2>&1" | getline filename )
      {
	if ( filename ~ "^\*\.3" )
	  break
	basename = filename
	sub( ".*/","",basename )
	sub( "\.[^.]*$", "", basename )
	linkfile = filename
	if ( match( linkfile, ".[hH][tT][mM][lL]?$") == 0 )
	  sub( "\.[^.]*$", ".html", linkfile )
	if ( output != "." )
	{
	  directory = linkfile
	  sub( "/.*","",directory )
	  if ( directory != linkfile )
	  {
	    if ( directory == output )
	      sub( "^[^/]*/","",linkfile )
	  }
	}
	names[basename] = linkfile
      }
    }

    initialised = 1
  }
}

$0 ~ "<H2>SYNOPSIS</H2>" {
  synopsis = 1
}

$0 ~ "^(class|struct|union) [^:]+ :" && synopsis == 1 {
  match( $0, "^(class|struct|union) [^:]+ :" )
  prefix = substr( $0, RSTART, RLENGTH )
  sub( "^(class|struct|union) [^:]+ :", "", $0 )
  printf( "%s", prefix )
  while ( $0 !~ "^{$" ) {
    $0 = addlinks( $0 )
    print $0
    if ( getline <= 0 )
      next
  }
  if ( $0 !~ "^{*$" )
    print $0

  synopsis = 0
}

$0 ~ "<H2>SEE ALSO</H2>" {
  print $0
  if ( getline <= 0 )
    next
  while ( $0 !~ "^<P>$" ) {
    $0 = addlinks( $0 )
    if ( $0 !~ "^[\t ]*$" )
      print $0
    if ( getline <= 0 )
      next
  }
  if ( $0 !~ "^[\t ]*$" )
    print $0
}

{ print $0 }

function addlinks( line ) {
  line = " " line " "
  narg = 1000
  pattern = "[^&_a-zA-Z0-9][_a-zA-Z][_a-zA-Z0-9]*[^_a-zA-Z0-9]"
  while ( match( line, pattern ) )
  {
    narg++
    arg = substr( line, RSTART+1, RLENGTH-2 )
    before = substr( line, RSTART, 1 )
    after = substr( line, RSTART+RLENGTH-1, 1 )
    # Need the following to stop resubstitution
    # of the pattern matched back into the string.
    gsub( "&", "\\\\&", after )
    sub( pattern, before "#" narg after, line )
    fargs["#" narg] = arg
  }
  for ( key in fargs )
  {
    arg = fargs[key]
    if ( arg in names )
      fargs[key] = "<A HREF=\"" names[arg] "\">" arg "</A>"
  }
  for ( key in fargs )
  {
    gsub( key, fargs[key], line )
  }
  sub( "^ ", "", line )
  sub( " $", "", line )
  return line
}
