# =============================================================================
#
# = DESCRIPTION
#     Awk script to convert classinfo description file to src stubs.
#
# = AUTHOR(S)
#     Graham Dumpleton
# 
# = COPYRIGHT
#     Copyright 1991 OTC LIMITED
#     Copyright 1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#
# =============================================================================

BEGIN {
  separator = "/* ------------------------------"\
   "------------------------------------------- */"
  FS="\n"; RS=""
}

$1 ~ /CLASS2INFO/ {
  printf( "#include \"%s\"\n\n%s\n", $3, separator )
}

$1 ~ /(CLASS|STRUCT|UNION)/ {
  names[$2] = $2
}

$1 ~ /TEMPLATE/ {
  class = $3 $4
  args[$3] = $4
  # This needs extra work.
  sub( "<[\t ]*(class)[\t ]+", "<", class )
  sub( ",[\t ]*(class)[\t ]+", ",", class )
  names[$3] = class
}

$1 ~ /VARIABLE/ {

  class = names[$2]
  prototype = $4

  # Filter out non static variables.
  if ( prototype !~ /^[\t ]*static[\t ]+/ )
    next

  sub( "^[\t ]*static[\t ]+", "", prototype )

  if ( prototype ~ "[a-zA-Z0-9_]+(\\[[\t a-zA-Z0-9_]*\\])?+;$" )
  {
    match( prototype, "[a-zA-Z0-9_]+(\\[[\t a-zA-Z0-9_]*\\])?+;$" )
    if ( RLENGTH != 0 )
    {
      newp = substr( prototype, 0, RSTART-1 )
      newp = newp class "::"
      newp = newp substr( prototype, RSTART )
      prototype = newp
    }
  }
  else if ( prototype ~ "\\([\t ]*\\*[\t ]*[a-zA-Z0-9_]+[\t ]*\\)[\t ]*\\(" )
  {
    match( prototype, "\\([\t ]*\\*[\t ]*[a-zA-Z0-9_]+[\t ]*\\)[\t ]*\\(" )
    tmpp = substr( prototype, RSTART, RLENGTH )
    gsub( "[\t ]+", "", tmpp )

    newp = substr( prototype, 0, RSTART-1 )
    newp = newp tmpp
    newp = newp substr( prototype, RSTART+RLENGTH )
    prototype = newp


    match( prototype, "\\(\\*[a-zA-Z0-9_]+\\)\\(" )
    if ( RLENGTH != 0 )
    {
      newp = substr( prototype, 0, RSTART+1 )
      newp = newp class "::"
      newp = newp substr( prototype, RSTART+2 )
      prototype = newp
    }
  }

  if ( prototype ~ /\[[\t ]*\];$/ )
    sub( "\\[[\t ]*\\]", "[...]", prototype )

  sub( ";$", " = ...;", prototype )

  if ( class ~ "<" )
    printf( "template%s\n", args[$2] )
  printf( "%s\n\n%s\n", prototype, separator )
}

$1 ~ /FUNCTION/ {

  class = names[$2]
  prototype = $4

  # Filter out inline functions.
  if ( prototype ~ /^[\t ]*inline[\t ]+/ )
    next

  # Filter out pure virtual functions.
  if ( prototype ~ /[\t ]*=[\t ]*0[\t ]*;[\t ]*$/ )
    next

  # Strip out unwanted bits.
  sub( "^[\t ]*(static|virtual)[\t ]+", "", prototype )
  sub( ";[\t ]*$", "", prototype )
  gsub( "[\t ]*=[^(=][^,=]*,", ",", prototype )
  gsub( "[\t ]*=[^(=][^,=]*\\)", ")", prototype )

  # Put in class name.
  if ( prototype ~ "^[\t ]*operator[\t ]+" )
    sub( "^[\t ]*", names[$2] "::", prototype )
  else
  {
    sub( "[\t ]*\\(", "(", prototype )
    match( prototype, "[^\t ]*\\(.*$" )
    decl = substr( prototype, RSTART, RLENGTH )
    sub( "^", names[$2] "::", decl )
    gsub( "&", "\\\\&", decl )
    sub( "[^\t ]*\\(.*$", decl, prototype )
  }

  # Output it.

  if ( length(prototype) > 75 )
  {
    match( prototype, "^[^(]*\\(" )
    funcname = substr( prototype, RSTART, RLENGTH )
    if ( prototype ~ "^[^(]*\\(\\)[\t ]*\\(" )
    {
      funcname = funcname ")("
      sub( "^[^(]*\\(\\)[\t ]*\\(", "", prototype )
    }
    else
      sub( "^[^(]*\\(", "", prototype )
    if ( class ~ "<" )
      printf( "template%s\n%s\n", args[$2], funcname )
    else
      printf( "%s\n", funcname )
    match( prototype, "\\)([\t ]*const)?[\t ]*$" )
    functail = substr( prototype, RSTART, RLENGTH )
    sub( "\\)([\t ]* const)?[\t ]*$", "", prototype )
    narg = 0
    while ( match( prototype, "[^<>,#]*<[^<>]*>" ) )
    {
      narg++
      arg = substr( prototype, RSTART, RLENGTH )
      sub( "[^<>,#]*<[^<>]*>", "#" narg, prototype )
      fargs["#" narg] = arg
      # Need the following to stop resubstitution of the pattern matched
      # back into the string.
      gsub( "&", "\\\\&", fargs["#" narg] )
    }
    numargs = split( prototype, args, "," )
    for ( m=1; m<=numargs; m++ )
    {
      while ( match( args[m], "#[0-9]+" ) )
      {
	arg = substr( args[m], RSTART, RLENGTH )
	sub( arg, fargs[arg], args[m] )
      }
      sub( "[\t ]*", "", args[m] )
      printf( " %s", args[m] )
      if ( m == numargs )
      {
	print( "" )
      }
      else
	print( "," )
    }
    printf( "%s\n{\n}\n\n%s\n", functail, separator )
  }
  else
  {
    if ( class ~ "<" )
      printf( "template%s\n", args[$2] )
    printf( "%s\n{\n}\n\n%s\n", prototype, separator )
  }
}
