#!/usr/bin/perl
use Template;

my $config = {
INTERPOLATE  => 1,		      # expand "$var" in plain text
POST_CHOMP	  => 1,		      # cleanup whitespace
EVAL_PERL	  => 1,		      # evaluate Perl code blocks
};

my $template = Template->new();
$Name=shift  or die("Usage: $0 Name\n");
#$Name="test";


$tpl_input_dir="tpl_wdm_drv";
$tpl_output_dir="wdm_drv"."_".$Name;

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


$template->process($tpl_input_dir."/HelloWDM.cpp.tpl", $vars, "$tpl_output_dir//${Name}WDM.cpp") || die $template->error();
$template->process($tpl_input_dir."/HelloWDM.h.tpl", $vars, "$tpl_output_dir//${Name}WDM.h") || die $template->error();
