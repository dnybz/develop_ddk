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


$tpl_input_dir="tpl_17_usb_sys";
$tpl_output_dir="output/tpl_17_usb_sys"."_".$Name;

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


$template->process($tpl_input_dir."/bulkusr.h.tpl", $vars, "$tpl_output_dir//bulkusr.h") || die $template->error();
$template->process($tpl_input_dir."/bulkrwr.h.tpl", $vars, "$tpl_output_dir//bulkrwr.h") || die $template->error();
$template->process($tpl_input_dir."/bulkpnp.h.tpl", $vars, "$tpl_output_dir//bulkpnp.h") || die $template->error();
$template->process($tpl_input_dir."/makefile.tpl", $vars, "$tpl_output_dir//makefile") || die $template->error();
$template->process($tpl_input_dir."/bulkwmi.c.tpl", $vars, "$tpl_output_dir//bulkwmi.c") || die $template->error();
$template->process($tpl_input_dir."/bulkrwr.c.tpl", $vars, "$tpl_output_dir//bulkrwr.c") || die $template->error();
$template->process($tpl_input_dir."/bulkdev.c.tpl", $vars, "$tpl_output_dir//bulkdev.c") || die $template->error();
$template->process($tpl_input_dir."/sources.tpl", $vars, "$tpl_output_dir//sources") || die $template->error();
$template->process($tpl_input_dir."/bulkusb.c.tpl", $vars, "$tpl_output_dir//bulkusb.c") || die $template->error();
$template->process($tpl_input_dir."/bulkpwr.c.tpl", $vars, "$tpl_output_dir//bulkpwr.c") || die $template->error();
$template->process($tpl_input_dir."/bulkusb.h.tpl", $vars, "$tpl_output_dir//bulkusb.h") || die $template->error();
$template->process($tpl_input_dir."/makefile.inc.tpl", $vars, "$tpl_output_dir//makefile.inc") || die $template->error();
$template->process($tpl_input_dir."/bulkusb.rc.tpl", $vars, "$tpl_output_dir//bulkusb.rc") || die $template->error();
$template->process($tpl_input_dir."/bulkpnp.c.tpl", $vars, "$tpl_output_dir//bulkpnp.c") || die $template->error();
$template->process($tpl_input_dir."/bulkusb.mof.tpl", $vars, "$tpl_output_dir//bulkusb.mof") || die $template->error();
$template->process($tpl_input_dir."/bulkwmi.h.tpl", $vars, "$tpl_output_dir//bulkwmi.h") || die $template->error();
$template->process($tpl_input_dir."/bulkusb.inf.tpl", $vars, "$tpl_output_dir//bulkusb.inf") || die $template->error();
$template->process($tpl_input_dir."/bulkusb.bmf.tpl", $vars, "$tpl_output_dir//bulkusb.bmf") || die $template->error();
$template->process($tpl_input_dir."/bulkpwr.h.tpl", $vars, "$tpl_output_dir//bulkpwr.h") || die $template->error();
$template->process($tpl_input_dir."/bulkdev.h.tpl", $vars, "$tpl_output_dir//bulkdev.h") || die $template->error();
