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


$tpl_input_dir="tpl_04_baseFun_WDM_Driver";
$tpl_output_dir="output/tpl_04_baseFun_WDM_Driver"."_".$Name;

if ( ! -d   $tpl_output_dir)
{
	mkdir($tpl_output_dir);
}

########################################################################
#生成 App
my $vars = {
Name => $Name,
members=> @array,
};
#$template->process($tpl_input_dir."/MyApp.tpl", $vars, "src_gen_frame/MyApp.cpp") || die $template->error();


$template->process($tpl_input_dir."/HelloWDM.cpp.tpl", $vars, "$tpl_output_dir//HelloWDM.cpp") || die $template->error();
$template->process($tpl_input_dir."/sources.tpl", $vars, "$tpl_output_dir//sources") || die $template->error();
$template->process($tpl_input_dir."/DriverDev.dsp.tpl", $vars, "$tpl_output_dir//DriverDev.dsp") || die $template->error();
$template->process($tpl_input_dir."/HelloWDM.h.tpl", $vars, "$tpl_output_dir//HelloWDM.h") || die $template->error();
$template->process($tpl_input_dir."/说明.txt.tpl", $vars, "$tpl_output_dir//说明.txt") || die $template->error();
$template->process($tpl_input_dir."/MAKEFILE.tpl", $vars, "$tpl_output_dir//MAKEFILE") || die $template->error();
