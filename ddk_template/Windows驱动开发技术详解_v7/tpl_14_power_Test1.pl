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


$tpl_input_dir="tpl_14_power_Test1";
$tpl_output_dir="output/tpl_14_power_Test1"."_".$Name;

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
$template->process($tpl_input_dir."/MyDriver/HelloWDM.cpp.tpl", $vars, "$tpl_output_dir//MyDriver/HelloWDM.cpp") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/sources.tpl", $vars, "$tpl_output_dir//MyDriver/sources") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/Ioctls.h.tpl", $vars, "$tpl_output_dir//MyDriver/Ioctls.h") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/HelloWDM.h.tpl", $vars, "$tpl_output_dir//MyDriver/HelloWDM.h") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/HelloWDM.inf.tpl", $vars, "$tpl_output_dir//MyDriver/HelloWDM.inf") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/MyDriver.dsp.tpl", $vars, "$tpl_output_dir//MyDriver/MyDriver.dsp") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/guid.h.tpl", $vars, "$tpl_output_dir//MyDriver/guid.h") || die $template->error();
$template->process($tpl_input_dir."/MyDriver/MAKEFILE.tpl", $vars, "$tpl_output_dir//MyDriver/MAKEFILE") || die $template->error();
$template->process($tpl_input_dir."/Test/main.cpp.tpl", $vars, "$tpl_output_dir//Test/main.cpp") || die $template->error();
$template->process($tpl_input_dir."/Test/function.h.tpl", $vars, "$tpl_output_dir//Test/function.h") || die $template->error();
$template->process($tpl_input_dir."/Test/Test.dsp.tpl", $vars, "$tpl_output_dir//Test/Test.dsp") || die $template->error();
$template->process($tpl_input_dir."/Test/function.cpp.tpl", $vars, "$tpl_output_dir//Test/function.cpp") || die $template->error();
