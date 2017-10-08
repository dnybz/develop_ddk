#!/usr/bin/perl 
#根据 tpl_give_a_name  的目录自动生成   give_a_name 
#!/usr/bin/perl 
my @tpls=grep{ -d $_}  glob("tpl_*");
my $input_tpl_dir=shift (@tpls) or die("当前目录下 没有 tpl_* 目录");
(my $output_tpl_dir)= ($input_tpl_dir=~/tpl_(.*)/);


print <<EOF
#!/usr/bin/perl
use Template;

my \$config = {
INTERPOLATE  => 1,		      # expand "\$var" in plain text
POST_CHOMP	  => 1,		      # cleanup whitespace
EVAL_PERL	  => 1,		      # evaluate Perl code blocks
};

my \$template = Template->new();
#\$Name=shift  or die("Usage: \$0 Name\\n");
\$Name="Netware";


\$tpl_input_dir="$input_tpl_dir";
\$tpl_output_dir="$output_tpl_dir"."_".\$Name;

if ( ! -d   \$tpl_output_dir)
{
	mkdir(\$tpl_output_dir);
}

########################################################################
#生成 App
	my \$vars = {
	Name => \$Name,
	members=> \@array,
	};
#\$template->process(\$tpl_input_dir."/MyApp.tpl", \$vars, "src_gen_frame/MyApp.cpp") || die \$template->error();


EOF
;

@tmps=glob($input_tpl_dir."/*.tpl");
for(@tmps)
{
	my $output_cpp=$_;
	my $full_path=$_;

	$output_cpp=~s/\.tpl//g;
	$output_cpp=~s/$input_tpl_dir//g;
	$full_path =~s/$input_tpl_dir//g;

	print <<EOF
	\$template->process(\$tpl_input_dir."$full_path", \$vars, "\$tpl_output_dir/$output_cpp") || die \$template->error();
EOF
;
}
