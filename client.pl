#!perl


#use SOAP::Lite +trace => 'all';
use SOAP::Lite;

use Data::Dumper;

$pingDataA = SOAP::Data->name(a => "http://www.cnn.com");

$calcDataA = SOAP::Data->name(a => $ARGV[0]);
$calcDataB = SOAP::Data->name(b => 2);

$structData = SOAP::Data->name(a => "a");

$res = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture')
    -> ping($pingDataA)
    -> result;

print Dumper($res);

