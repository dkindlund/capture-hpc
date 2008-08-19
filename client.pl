#!perl

use Data::Dumper;

use SOAP::Lite +trace => 'all';
#use SOAP::Lite;
use SOAP::Lite::Packager;
use MIME::Entity;
use MIME::Base64;

#test with a binary file
open(A, $ARGV[0]) or die "Can't open $ARGV[0]\n";
$fullfile = "";
while(<A>){
	$fullfile .= $_;
}
#$encoded = encode_base64($fullfile);
#print "$fullfile\n";
#print "$encoded\n";
@statz = stat($ARGV[0]);
$decodedLength = $statz[7];
if(length($fullfile) != $decodedLength){
print "What tha...\n";
}
#$encodedLength = length($encoded);
#print "File size = " . $statz[7] . "\n";
#print "Base64 size = " . length($encoded)  . "\n";

my $ent = build MIME::Entity
	Type	=> "application/exe",
	Path	=> $ARGV[0],
	Filename => $ARGV[0],
	Disposition => "attachment";


#$base64Data = SOAP::Data->name(data => $encoded, encodedLength => $encodedLength, decodedLength => $decodedLength);
#$base64Data1 = SOAP::Data->name(data => $fullfile);
$base64Data1 = SOAP::Data->name(data => "abcdefg");
$base64Data2 = SOAP::Data->name(encodedLength => 0);
#$base64Data2 = SOAP::Data->name(encodedLength => 7);
$base64Data3 = SOAP::Data->name(decodedLength => $decodedLength);
#$base64Data3 = SOAP::Data->name(decodedLength => 8);

$pingDataA = SOAP::Data->name(a => "http://www.cnn.com");

$calcDataA = SOAP::Data->name(a => $ARGV[0]);
$calcDataB = SOAP::Data->name(b => 2);

$structData = SOAP::Data->name(a => "a");

$res = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture')
    -> ping($pingDataA)
    -> result;


#$res = SOAP::Lite
#    -> proxy('http://192.168.0.131:1234/')
#    -> ns('capture')
#    -> junks($structData)
#    -> result;

print "\ncalling client setup\n\n";
#    -> packager(SOAP::Lite::Packager::MIME->new)
#    -> parts([$ent])
$client = SOAP::Lite
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture');

print "\ncalling sendBase64\n\n";
$result = $client->sendBase64($base64Data1, $base64Data2, $base64Data3);
#$result = $client->sendMIME(31337);

#print Dumper($result);

print "calling junks\n";
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











