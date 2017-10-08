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


$tpl_input_dir="tpl_03_loadDrv_LoadNTDriver";
$tpl_output_dir="output/tpl_03_loadDrv_LoadNTDriver"."_".$Name;

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


$template->process($tpl_input_dir."/DriverDev.dsw.tpl", $vars, "$tpl_output_dir//DriverDev.dsw") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/sources.tpl", $vars, "$tpl_output_dir//MyDriver/sources") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/buildfre.log.tpl", $vars, "$tpl_output_dir//MyDriver/buildfre.log") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/Driver.h.tpl", $vars, "$tpl_output_dir//MyDriver/Driver.h") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/Driver.cpp.tpl", $vars, "$tpl_output_dir//MyDriver/Driver.cpp") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/MyDriver.dsp.tpl", $vars, "$tpl_output_dir//MyDriver/MyDriver.dsp") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/MAKEFILE.tpl", $vars, "$tpl_output_dir//MyDriver/MAKEFILE") || die $template->error();
$template->process($tpl_input_dir."/Test/makefile.vc.tpl", $vars, "$tpl_output_dir//Test/makefile.vc") || die $template->error();
$template->process($tpl_input_dir."/Test/Test.exe.tpl", $vars, "$tpl_output_dir//Test/Test.exe") || die $template->error();
$template->process($tpl_input_dir."/Test/main.cpp.tpl", $vars, "$tpl_output_dir//Test/main.cpp") || die $template->error();
$template->process($tpl_input_dir."/Test/sources.tpl", $vars, "$tpl_output_dir//Test/sources") || die $template->error();
$template->process($tpl_input_dir."/Test/Test.dsp.tpl", $vars, "$tpl_output_dir//Test/Test.dsp") || die $template->error();
