#!perl

use Data::Dumper;

use SOAP::Lite +trace => 'all';
#use SOAP::Lite;

#NOTE: the first parameter of name() must exactly match the variable name in captureGSOAP.h
$maxEventsToReturnSOAP = SOAP::Data->name(maxEventsToReturn => $ARGV[0]);

print "\ncalling returnEvents\n\n";
$client = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture')
    -> returnEvents($maxEventsToReturnSOAP)
    -> result;







