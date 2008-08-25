#!perl

use Data::Dumper;

use SOAP::Lite +trace => 'all';
#use SOAP::Lite;

$fileNameSOAP = SOAP::Data->name(fileName => $ARGV[0]);
$waitTimeMillisecSOAP = SOAP::Data->name(waitTimeMillisec => 20000);


print "\ncalling openDocument\n\n";
$client = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture')
    -> openDocument($fileNameSOAP, $waitTimeMillisecSOAP)
    -> result;

print "openDocument returned $result\n";








