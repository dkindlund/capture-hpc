#!perl

use Data::Dumper;

use SOAP::Lite +trace => 'all';
#use SOAP::Lite;

#NOTE: the first parameter of name() must exactly match the variable name in captureGSOAP.h
$urlSOAP = SOAP::Data->name(URL => $ARGV[0])->type('string');

print "\ncalling visitURL\n\n";
$client = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture')
    -> visitURL($urlSOAP)
    -> result;







