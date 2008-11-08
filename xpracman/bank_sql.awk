#!/usr/bin/awk -f
#
#     $Id: bank_sql.awk 1032 2001-12-10 04:23:37Z source $
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

# Function to extract bank report data from PRAC database
#
# 

function bank_sql(dbname, bkdv_bkdp__sequence) {
    tmpfile = "bank.sql";

# Arrays to access data by attribute name

## BKDV




    bkdv["bkdv_bkdp__sequence"] = 1;
    bkdv["bkdv_date_entry"] = 2;
    bkdv["bkdv_user_entry"] = 3;
    bkdv["bkdv_site_entry"] = 4;
    bkdv["bkdv_paym_amount"] = 5;
    bkdv["bkdv_tdtp_code"] = 6;
    bkdv["bkdv_tdtp_list"] = 7;
    bkdv["bkdv_tdtp_subtotal"] = 8;
    bkdv["bkdv_tdtp_desc"] = 9;
    bkdv["bkdv_tdtp_entity"] = 10;
    bkdv["bkdv_tdtp_location"] = 11;
    bkdv["bkdv_drawer"] = 12;
    bkdv["bkdv_bank"] = 13;
    bkdv["bkdv_branch"] = 14;
    bkdv["bkdv_paym__sequence"] = 15;
    bkdv["bkdv_invc__sequence"] = 16;
    bkdv["bkdv_cred_amount"] = 17;
    bkdv["bkdv_cred_gst_amount"] = 18;
    bkdv["bkdv_patn__sequence"] = 19;
    bkdv["bkdv_patn_psnam"] = 20;
    bkdv["bkdv_patn_fsnam"] = 21;
    bkdv["bkdv_dbtr_code"] = 22;
    bkdv["bkdv_dbtr_name"] = 23;
    bkdv["bkdv_bank_code"] = 24;
    bkdv["bkdv_bank_name"] = 25;
    bkdv["bkdv_bank_address"] = 26;
    bkdv["bkdv_bank_suburb"] = 27;
    bkdv["bkdv_bank_state"] = 28;
    bkdv["bkdv_bank_postcode"] = 29;
    bkdv["bkdv_bank_phone"] = 30;
    bkdv["bkdv_bank_bank"] = 31;
    bkdv["bkdv_bank_branch"] = 32;
    bkdv["bkdv_bank_account"] = 33;
    bkdv["bkdv__sequence"] = 34;
    bkdv["tdtp_desc"] = 35;
    
    
# An index to get the order the same as lists

    bkdv_count = 0;
    for ( lab in bkdv )
    {
        bkdv_idx[bkdv[lab]] = lab;
        bkdv_count++;
    }
    
# build attribute lists for the select statement.
    bkdv_attlist = "'bkdv'";
    for (i = 1; i <= bkdv_count; i++)
        bkdv_attlist = sprintf("%s,%s", bkdv_attlist, bkdv_idx[i]);

# prepare the sql script
    printf("\\pset tuples_only\n" \
           "\\pset format unaligned\n" \
           " -- bkdv\n" \
           "select     %s\n" \
           " from      bkdv LEFT JOIN tdtp on (bkdv_tdtp_code = tdtp_code) \n" \
           " where     bkdv_bkdp__sequence = %d" \
           " and       bkdv_paym_amount > 0" \
           " order by  bkdv_bank_code,bkdv_tdtp_list,bkdv_tdtp_subtotal,\
                       bkdv_tdtp_code,bkdv_paym__sequence,bkdv__sequence;\n",
           bkdv_attlist,
           bkdv_bkdp__sequence) > tmpfile;
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
