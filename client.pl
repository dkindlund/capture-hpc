#!perl

use Data::Dumper;

#use SOAP::Lite +trace => 'all';
use SOAP::Lite;
use SOAP::MIME;
use MIME::Entity;


$pingDataA = SOAP::Data->name(a => "http://www.cnn.com");

$calcDataA = SOAP::Data->name(a => $ARGV[0]);
$calcDataB = SOAP::Data->name(b => 2);

$structData = SOAP::Data->name(a => "a");

$res = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture')
    -> ping($pingDataA)
    -> result;


$res = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture')
    -> junks($structData)
    -> result;

print Dumper($res);


$res = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture');

my $som = $res->junks($structData);

print "ns__myStruct.first = " . $som->result . "\n";
print "ns__myStruct.last = " . ($som->paramsout)[0] . "\n";

my @params = $som->paramsall;
foreach $param (@params){
    print "$param\n";

}











