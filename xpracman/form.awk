#!/usr/bin/awk -f
#
#     $Id: form.awk 464 2001-05-29 22:54:54Z source $
# 
#   Copyright 2001 X=X Computer Software Trust
#    		  Kangaroo Ground Australia 3097
# 
# 
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#  
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this software; see the file COPYING.  If not, write to
#  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
#  
#  Report problems and direct all questions to:
#  
#      Rex McMaster, rmcm@compsoft.com.au
#  
# 

# Script to print invoice from PRAC schema
#
# usage: env DBNAME=prac patn__sequence=5002 invoice.awk

function set_output(output)
{
    if (!output)
        return "";
    gsub(/\$YYMMDD/, "`date +%y%m%d`", output);
    gsub(/\$USER/, "`logname`", output);
    gsub(/\$HHMM/, "`date +%H%M`", output);
# If not pipe, then use cat to standardize output to pipe
    if (!gsub(/^\|/, "", output))
    {
        if (gsub(/^-/, "", output))
        {
            realfilename = output;
            output = "cat > "output;
        }
        else
        {
            realfilename = output;
            output = "cat >> "output;
        }
    }
    return output;
}

BEGIN {
    dbname = ENVIRON["DBNAME"];
    patn__sequence = ENVIRON["patn__sequence"];
    layout_name = ENVIRON["layout_name"];
    
    if ( dbname == "" || patn__sequence == "" || layout_name == "" )
        exit 1;
    
    ## Load the form data
    form_sql( dbname, patn__sequence);
    exit; ## Jump to END{}
}

END     {
## Initialise layout variables.
    initformat();

## Load the data into arrays
    split( inputline["patn", 1], patn_fields, "|" );
    split( inputline["dbtr", 1], dbtr_fields, "|" );
    split( inputline["prov", 1], prov_fields, "|" );
    split( inputline["empl", 1], empl_fields, "|" );
    split( inputline["rfdr", 1], rfdr_fields, "|" );


# Read the form file
#
    init_flag = 0;
    while (getline < layout_name > 0)
    {
        if ($0 ~ /^[ \t]*\#/)
            continue;
        if ($0 ~ /^OUTPUT=/)
        {
            gsub(/^OUTPUT=/, "", $0);
            if (!output)
            {
                output = $0;
                output = set_output(output);
            }
            continue;
        }
# If we get to here, then an output should be defined.
        if (!output)
            output = "cat";
        if ( !init_flag )
        {
            printf "%s%s", init_pr, font_fx_10 | output;
            init_flag = 1;
        }
            
        for (j = 1; j <= NF;  j++)
        {
            if (j % 2)
            {
                gsub(/\$TODAY/, today, $j);
                gsub(/\$TIME/, now, $j);
                gsub(/\$USER/, logname, $j);
                printf "%s", $j | output;
            }
            else
            {
                ## Kludgy data switch
                tabname = $j;
                gsub(/_.*/, "", tabname);
                if ( tabname == "patn" )
                    data = patn_fields[patn[$j]];
                else
                    if (tabname == "dbtr" )
                        data = dbtr_fields[dbtr[$j]];
                    else
                        if (tabname == "prov" )
                            data = prov_fields[prov[$j]];
                        else
                            if (tabname == "empl" )
                                data = empl_fields[empl[$j]];
                            else
                                if (tabname == "rfdr" )
                                    data = rfdr_fields[rfdr[$j]];

                        
                printf "%s", data | output;
            }
        }
        printf "\n" | output;
    }
# If post-create action and a destination file exist
# then do it.
    if (action && realfilename)
    {
        printf action, realfilename > commandfile;
    }

}
