#n

# Copyright 1995 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

# Use a script file for sed commands as MKS version of sed on Windows NT
# wouldn't work when run from inside makeit with the required patterns.
# The patterns would work when MKS version of sed was run from MKS Korn
# Shell explicitly.

# Convert DOS backslash to normal slash. Note that this is done to all
# lines, same with the subsequent substitution.

s%\\%/%g

# Replace multiple consecutive occurences of slash with a single slash.

s%//%/%g

# Massage the lines generated by the preprocessor which are interested in.

s%^ *\# *[1-9][0-9]* "\(.*\)".*$%#TARGET : \1%
s%^ *\#line [1-9][0-9]* "\(.*\)".*$%#TARGET : \1%

# Remove the lines which have a drive specifier in them.

/^#TARGET : [a-zA-Z]:.*$/d

# Remove strange lines on MAC OS X such as <built-in> and <command line>.

/^#TARGET : [<].*$/d

# Print out those finally left which we are interested in.

s%^#\(TARGET.*\)$%\1%p