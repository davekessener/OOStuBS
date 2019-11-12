#!/usr/bin/env ruby

require 'fileutils'

src = Dir.glob('ut_*.cc')

File.open('main.cc', 'w') do |f|
	f.write(<<EOF
#include <iostream>

#include "ut.h"

#{src.map do |fn|
	dep = []
	File.open(fn, 'r', &:read).split(/\n/).each do |line|
		if line =~ /#include[ \t]+([<"][^>"]+[>"])/
			dep.push($1)
		end
	end
	dep
end.flatten.uniq.map do |dep|
	"#include #{dep}"
end.join("\n")}

int main(int, char **)
{
	using namespace oostubs;

#{src.map { |s| "#include \"#{s}\"" }.join("\n")}

	UTManager::instance().run();

	return 0;
}

EOF
)
end

