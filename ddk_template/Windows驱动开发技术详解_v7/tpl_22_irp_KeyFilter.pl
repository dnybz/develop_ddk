#!/usr/bin/perl
use Template;

my $config = {
INTERPOLATE  => 1,		      # expand "$var" in plain text
POST_CHOMP	  => 1,		      # cleanup whitespace
EVAL_PERL	  => 1,		      # evaluate Perl code blocks
};

my $template = Template->new();
$Name=shift  or die("Usage: $0 Name\n");
#$Name="Netware";


$tpl_input_dir="tpl_22_irp_KeyFilter";
$tpl_output_dir="output/tpl_22_irp_KeyFilter"."_".$Name;

if ( ! -d   $tpl_output_dir)
{
	mkdir($tpl_output_dir);
}

########################################################################
#Éú³É App
my $vars = {
Name => $Name,
members=> @array,
};
#$template->process($tpl_input_dir."/MyApp.tpl", $vars, "src_gen_frame/MyApp.cpp") || die $template->error();


$template->process($tpl_input_dir."/SOURCES.tpl", $vars, "$tpl_output_dir//SOURCES") || die $template->error();
$template->process($tpl_input_dir."/ctrl2cap.rc.tpl", $vars, "$tpl_output_dir//ctrl2cap.rc") || die $template->error();
$template->process($tpl_input_dir."/ctrl2cap.c.tpl", $vars, "$tpl_output_dir//ctrl2cap.c") || die $template->error();
$template->process($tpl_input_dir."/CTRL2CAP.H.tpl", $vars, "$tpl_output_dir//CTRL2CAP.H") || die $template->error();
$template->process($tpl_input_dir."/MAKEFILE.tpl", $vars, "$tpl_output_dir//MAKEFILE") || die $template->error();
