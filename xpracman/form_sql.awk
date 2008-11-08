#!/usr/bin/awk -f
#
#     $Id: form_sql.awk 1705 2003-04-27 08:55:02Z source $
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

# Function to extract patient demographics from PRAC database
#
# 

function form_sql(dbname, patn__sequence) {
    tmpfile = "form.sql";

# Arrays to access data by attribute name
    
## DBTR
    dbtr["dbtr_code"] = 1;
    dbtr["dbtr_name"] = 2;
    dbtr["dbtr_address"] = 3;
    dbtr["dbtr_suburb"] = 4;
    dbtr["dbtr_postcode"] = 5;
    dbtr["dbtr_state"] = 6;
    dbtr["dbtr_phone"] = 7;

## PATN
    patn["patn_flno"] = 1;
    patn["patn_psnam"] = 2;
    patn["patn_fsnam"] = 3;
    patn["patn_title"] = 4;
    patn["patn_dob"] = 5;
    patn["patn_address"] = 6;
    patn["patn_suburb"] = 7;
    patn["patn_state"] = 8;
    patn["patn_postcode"] = 9;
    patn["patn_phone"] = 10;
    patn["patn_hlfd_code"] = 11;
    patn["patn_ins_level"] = 12;
    patn["patn_healthnumb"] = 13;
    patn["patn_feet_code"] = 14;
    patn["patn_medicare"] = 15;
    patn["patn_healthcard"] = 16;
    patn["patn__sequence"] = 17;
    patn["patn_ref_date"] = 18;
    patn["patn_age"] = 19;

## PROV
    
    prov["prov_code"] = 1;
    prov["prov_provider_num"] = 2;
    prov["prov_name"] = 3;
    prov["prov_address"] = 4;
    prov["prov_suburb"] = 5;
    prov["prov_state"] = 6;
    prov["prov_postcode"] = 7;
    prov["prov_salutation"] = 8;
    prov["prov_phone"] = 9;
    prov["prov_bank_code"] = 10;
    prov["prov__sequence"] = 11;
    prov["prov__timestamp"] = 12;
    prov["prov__user_entry"] = 13;
    prov["prov__status"] = 14;
    
## EMPL
    empl["empl_code"] = 1;
    empl["empl_name"] = 2;
    empl["empl_address"] = 3;
    empl["empl_suburb"] = 4;
    empl["empl_postcode"] = 5;
    empl["empl_state"] = 6;

## RFDR
    
    rfdr["rfdr_code"] = 1;
    rfdr["rfdr_name"] = 2;
    rfdr["rfdr_street"] = 3;
    rfdr["rfdr_suburb"] = 4;
    rfdr["rfdr_postcode"] = 5;
    rfdr["rfdr_state"] = 6;
    rfdr["rfdr_provider"] = 7;
    rfdr["rfdr_phone"] = 8;
    rfdr["rfdr_salutation"] = 9;
    rfdr["rfdr_index"] = 10;

    
# An index to get the order the same as lists
    dbtr_count = 0;
    for ( lab in dbtr )
    {
        dbtr_idx[dbtr[lab]] = lab;
        dbtr_count++;
    }
    patn_count = 0;
    for ( lab in patn )
    {
        patn_idx[patn[lab]] = lab;
        patn_count++;
    }
    prov_count = 0;
    for ( lab in prov )
    {
        prov_idx[prov[lab]] = lab;
        prov_count++;
    }
    empl_count = 0;
    for ( lab in empl )
    {
        empl_idx[empl[lab]] = lab;
        empl_count++;
    }
    rfdr_count = 0;
    for ( lab in rfdr )
    {
        rfdr_idx[rfdr[lab]] = lab;
        rfdr_count++;
    }
    
# build attribute lists for the select statement.
    dbtr_attlist = "'dbtr'";
    for (i = 1; i <= dbtr_count; i++)
        dbtr_attlist = sprintf("%s,%s", dbtr_attlist, dbtr_idx[i]);
    patn_attlist = "'patn'";
    for (i = 1; i <= patn_count; i++)
        patn_attlist = sprintf("%s,%s", patn_attlist, patn_idx[i]);
    gsub(/patn_dob/, "date(patn_dob)", patn_attlist);
    gsub(/patn_ref_date/, "date(patn_ref_date)", patn_attlist);
    gsub(/patn_age/, "extract(year from age(patn_dob))||'.'||extract(month from age(patn_dob)) as patn_age", patn_attlist);
    prov_attlist = "'prov'";
    for (i = 1; i <= prov_count; i++)
        prov_attlist = sprintf("%s,%s", prov_attlist, prov_idx[i]);
    empl_attlist = "'empl'";
    for (i = 1; i <= empl_count; i++)
        empl_attlist = sprintf("%s,%s", empl_attlist, empl_idx[i]);
    rfdr_attlist = "'rfdr'";
    for (i = 1; i <= rfdr_count; i++)
        rfdr_attlist = sprintf("%s,%s", rfdr_attlist, rfdr_idx[i]);

# prepare the sql script
    printf("\\pset tuples_only\n" \
           "\\pset format unaligned\n" \
           " -- patn\n" \
           "select     %s\n" \
           " from      patn\n" \
           " where     patn__sequence = %d;\n" \
           " -- dbtr\n" \
           "select     %s\n" \
           " from      patn,dbtr\n" \
           " where     patn__sequence = %d\n" \
           " and       dbtr_code = patn_dbtr_code;\n" \
           " -- prov\n" \
           "select     %s\n" \
           " from      patn,prov\n" \
           " where     patn__sequence = %d\n" \
           " and       prov_code = patn_prov_code;\n" \
           " -- empl\n" \
           "select     %s\n" \
           " from      patn,empl\n" \
           " where     patn__sequence = %d\n" \
           " and       empl_code = patn_empl_code;\n" \
           " -- rfdr\n" \
           "select     %s\n" \
           " from      patn,rfdr\n" \
           " where     patn__sequence = %d\n" \
           " and       rfdr_code = patn_rfdr_code;\n",
           patn_attlist,
           patn__sequence,
           dbtr_attlist,
           patn__sequence,
           prov_attlist,
           patn__sequence,
           empl_attlist,
           patn__sequence,
           rfdr_attlist,
           patn__sequence) > tmpfile;
    close(tmpfile);
    
    psql =  sprintf("psql -q -d %s -f %s", dbname, tmpfile );

    count = 0;
    FS = "|";
    while ( psql | getline > 0 )
    {
        table = $1;
        table_count[table]++;
        sub(/^[^|]*\|/, "", $0);
        inputline[table,table_count[table]] = $0;
    }
    close(psql);

}
