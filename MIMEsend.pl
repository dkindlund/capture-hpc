#!perl

use SOAP::Lite +trace => 'all';
#use SOAP::Lite;
use SOAP::Lite::Packager;
use MIME::Entity;

my $ent = build MIME::Entity
	Type	=> "application/exe",
	Path	=> $ARGV[0],
	Filename => $ARGV[0],
	Disposition => "attachment";

print "\nCalling client setup\n\n";
$client = SOAP::Lite
    -> packager(SOAP::Lite::Packager::MIME->new)
    -> parts([$ent])
    -> proxy('http://192.168.0.131:1234/')
    -> ns('capture');

print "\nCalling sendMIME\n\n";
$data = SOAP::Data->name(magicNumber => "123");
$client->sendMIME($data);
#$result = $client->sendMIME($data);
#print "result = $result\n";








