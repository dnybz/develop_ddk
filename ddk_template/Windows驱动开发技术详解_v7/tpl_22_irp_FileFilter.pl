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


$tpl_input_dir="tpl_22_irp_FileFilter";
$tpl_output_dir="output/tpl_22_irp_FileFilter"."_".$Name;

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


$template->process($tpl_input_dir."/filter.inf.tpl", $vars, "$tpl_output_dir//filter.inf") || die $template->error();
$template->process($tpl_input_dir."/stddcls.h.tpl", $vars, "$tpl_output_dir//stddcls.h") || die $template->error();
$template->process($tpl_input_dir."/MyFilter.dsw.tpl", $vars, "$tpl_output_dir//MyFilter.dsw") || die $template->error();
$template->process($tpl_input_dir."/Driver.h.tpl", $vars, "$tpl_output_dir//Driver.h") || die $template->error();
$template->process($tpl_input_dir."/stddcls.cpp.tpl", $vars, "$tpl_output_dir//stddcls.cpp") || die $template->error();
$template->process($tpl_input_dir."/MyFilter.dsp.tpl", $vars, "$tpl_output_dir//MyFilter.dsp") || die $template->error();
$template->process($tpl_input_dir."/DriverEntry.cpp.tpl", $vars, "$tpl_output_dir//DriverEntry.cpp") || die $template->error();
