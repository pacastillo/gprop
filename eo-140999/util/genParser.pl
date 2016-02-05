#!/usr/bin/perl

#file format
#Program description
#Var_type (int,float,string or bool) var_name var_default_value Var description
#For instance
#SGA.vars
#Implementation of a simple genetic algorithm
#int popSize 20 Population size
#int numGenerations 20 Number of generations to run the algorithm
#float mutRate 0.2 Number of chromosomes mutated
#float xOverRate 0.8 Number of chromosomes to undergo crossover
#bool verbose true Run verbosely

#Generates a .h and .cpp file to parse those params, and prints out the 
#command line to call it. The name of the variables in the file should be
#the same than in the program

die "No var file found" if $#ARGV < 0;

open( VARS, "<$ARGV[0]");
@vars = <VARS>;
close VARS;

chop( $progDescription = shift @vars);

#Process param file
foreach ( @vars ) {
  ($type, $name, $defValue, $descr ) = /(\w+)\s+(\w+)\s+(\S+)\s+(.+)$/;
  $types{$name}=$type;
  $defValue{$name}=$defValue;
  $descr{$name}=$descr;
}

#Get file names
($fileName)=($ARGV[0]=~/(\w+)\./);

#Print header file
print "Generating $fileName-getParams.h\n";
open( DOTH, ">$fileName-getParams.h");
&printSignature( DOTH );
print DOTH<<EOC;
#ifndef GETPARAMS_H
#define GETPARAMS_H
EOC
&printDeclaration( DOTH );
print DOTH ";\n#endif\n";
close DOTH;

#Print definition file
print "Generating $fileName-getParams.cpp\n";
open( DOTCPP, ">$fileName-getParams.cpp");
&printSignature( DOTCPP );
print DOTCPP<<EOC;
#include "$fileName-getParams.h"
#include <util/Parser.h>

/// Parses the program´s command line, by creating a parsing object
EOC

&printDeclaration( DOTCPP );
print DOTCPP<<EOC;
{
// Create the command-line parser
  Parser parser( _argv[0], "$progDescription");
  parser.addFlag("-h", "--help", "Shows this help");
EOC

#Params stores the letters of the parametsr
$params{"h"} = 1;
foreach ( keys %types ) {
  print DOTCPP "  parser.add";
  #Search for parameter name
  $idxChar = 0;
  do {
    $firstChar = substr($_, $idxChar++, 1);
  } while ( $params{$firstChar} && $idxChar <= length( $_ ) );
  die "Couldn't find a suitable character for ".$_ if $idxChar > length( $_ );
  $params{$firstChar} = 1;
  $firstChar{$_} = $firstChar;
  #Now add params to the parser
  if ( $types{$_} eq "bool" ) {
    print DOTCPP<<EOC;
Flag("-$firstChar", "--$_", "$descr{$_}");
EOC
  }

  $types{ $_ } = "int" if ( $types{$_} eq "unsigned" );
  
  if ( $types{$_} =~ /(int|float)/ ) {
    $types{$_} =~ tr/[a-z]/[A-Z]/;
    print DOTCPP<<EOC;
Param("-$firstChar", "--$_", "$defValue{$_}", Param::$types{$_},"$descr{$_}");
EOC
  }
}


#Now check the parameter values
  print DOTCPP<<EOC;
  try {
    parser.parse(_argc, _argv);		// Do the parsing
EOC
foreach ( keys %types ) {
  
  if ( $types{$_} eq "bool" ) {
    print DOTCPP<<EOC;
    _$_ = parser.isEnabled("-$firstChar{$_}");
EOC
  }

  if ( $types{$_} =~ /(INT|FLOAT)/ ) {
    #Convert again the type names
    substr($types{$_},1, length($types{$_})) =~ tr/[A-Z]/[a-z]/;
    print DOTCPP<<EOC;
    _$_ = parser.get$types{$_}("-$firstChar{$_}");
EOC
  }
    
  
}

#Now check for help and print the rest
  print DOTCPP<<EOC;
    if (parser.isEnabled("-h") || parser.isEnabled("--help") ) {
      parser.printHelp();
      exit(1);
    }
  }
  catch (UException & e){
    cout << e.what() << endl;
    parser.printHelp();
    exit(1);
  }
}
EOC
close DOTCPP;

#Print the line to be included in the caller
print "Call it using\ngetParams( argc, argv, ";
@keysTypes = keys %types;
for ( $i = 0; $i <= $#keysTypes; $i++) {
  print   " $keysTypes[$i]";
  print "," if ($i <  $#keysTypes);
}
print ");\n";
print "Done\n";
  


#-----------------------------------
sub printSignature {
  local( *FILE ) = $_[0];
  print FILE "// This file generated automagically by genParser.pl from $fileName\n// genParser (c) JJ Merelo, 1999\n";
}

#-----------------------------------
sub printDeclaration {
  local( *FILE ) = $_[0];
  print FILE "void getParams( int _argc, char** _argv, \n";
  @keysTypes = keys %types;
  for ( $i = 0; $i <= $#keysTypes; $i++) {
    print  FILE "\t\t$types{$keysTypes[$i]} \& _$keysTypes[$i]";
    print FILE ",\n" if $i <  $#keysTypes;
  }

  print FILE ")";
}
