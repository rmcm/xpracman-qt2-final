#!/usr/bin/awk -f
#
#     $Id: invoice.awk 1478 2002-11-19 09:30:25Z source $
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
# usage: env DBNAME=prac invc__sequence=5002 invoice.awk 

BEGIN {
    dbname = ENVIRON["DBNAME"];
    invc__sequence = ENVIRON["invc__sequence"];
    if ( dbname == "" || invc__sequence == "")
        exit 1;
    tmpfile = "invoice.sql";

# Arrays to access data by attribute name

## INVC
    
    invc["invc_dbtr_code"] = 1;
    invc["invc_bank_code"] = 2;
    invc["invc_prov_code"] = 3;
    invc["invc_patn__sequence"] = 4;
    invc["invc_empl_code"] = 5;
    invc["invc_feet_code"] = 6;
    invc["invc_rfdr_code"] = 7;
    invc["invc_rfdr_date"] = 8;
    invc["invc_rfdr_period"] = 9;
    invc["invc_date_created"] = 10;
    invc["invc_date_printed"] = 11;
    invc["invc_date_reprint"] = 12;
    invc["invc_amount"] = 13;
    invc["invc_paid_amount"] = 14;
    invc["invc_gst_amount"] = 15;
    invc["invc_paid_gst_amount"] = 16;
    
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

## SVPF
    
    svpf["svpf_date_service"] = 1;
    svpf["svpf_serv_code"] = 2;
    svpf["svpf_percentage"] = 3;
    svpf["svpf_desc"] = 4;
    svpf["svpf_amount"] = 5;
    svpf["svpf_gst_amount"] = 6;
    svpf["svpf_invc__sequence"] = 7;
    svpf["svpf_mdaf__sequence"] = 8;
    svpf["svpf_patn__sequence"] = 9;

## CREV
    crev["crev_date_entry"] = 1;
    crev["crev_user_entry"] = 2;
    crev["crev_site_entry"] = 3;
    crev["crev_paym_amount"] = 4;
    crev["crev_tdtp_code"] = 5;
    crev["crev_drawer"] = 6;
    crev["crev_bank"] = 7;
    crev["crev_branch"] = 8;
    crev["crev_paym__sequence"] = 9;
    crev["crev_invc__sequence"] = 10;
    crev["crev_cred_amount"] = 11;
    crev["crev_cred_gst_amount"] = 12;
    crev["crev__sequence"] = 13;
    
# An index to get the order the same as lists
    invc_count = 0;
    for ( lab in invc )
    {
        invc_idx[invc[lab]] = lab;
        invc_count++;
    }
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
    svpf_count = 0;
    for ( lab in svpf )
    {
        svpf_idx[svpf[lab]] = lab;
        svpf_count++;
    }
    crev_count = 0;
    for ( lab in crev )
    {
        crev_idx[crev[lab]] = lab;
        crev_count++;
    }
    
# build attribute lists for the select statement.
    invc_attlist = "'invc'";
    for (i = 1; i <= invc_count; i++)
        invc_attlist = sprintf("%s,%s", invc_attlist, invc_idx[i]);
    gsub(/invc_rfdr_date/, "(case when invc_rfdr_date is not null then to_char(invc_rfdr_date,'DD-MM-YYYY') else '' end)",    invc_attlist);
    dbtr_attlist = "'dbtr'";
    for (i = 1; i <= dbtr_count; i++)
        dbtr_attlist = sprintf("%s,%s", dbtr_attlist, dbtr_idx[i]);
    patn_attlist = "'patn'";
    for (i = 1; i <= patn_count; i++)
        patn_attlist = sprintf("%s,%s", patn_attlist, patn_idx[i]);
    gsub(/patn_dob/, "short_date(patn_dob) as patn_dob", patn_attlist);
    prov_attlist = "'prov'";
    for (i = 1; i <= prov_count; i++)
        prov_attlist = sprintf("%s,%s", prov_attlist, prov_idx[i]);
    empl_attlist = "'empl'";
    for (i = 1; i <= empl_count; i++)
        empl_attlist = sprintf("%s,%s", empl_attlist, empl_idx[i]);
    rfdr_attlist = "'rfdr'";
    for (i = 1; i <= rfdr_count; i++)
        rfdr_attlist = sprintf("%s,%s", rfdr_attlist, rfdr_idx[i]);
    svpf_attlist = "'svpf'";
    for (i = 1; i <= svpf_count; i++)
        svpf_attlist = sprintf("%s,%s", svpf_attlist, svpf_idx[i]);
    gsub(/svpf_date_service/, "(case when svpf_date_service is not null then to_char(svpf_date_service,'DD-MM-YYYY') else '' end)", svpf_attlist);
    crev_attlist = "'crev'";
    for (i = 1; i <= crev_count; i++)
        crev_attlist = sprintf("%s,%s", crev_attlist, crev_idx[i]);
    gsub(/crev_date_entry/, "(case when crev_date_entry is not null then to_char(crev_date_entry,'DD-MM-YYYY') else '' end)", crev_attlist);

# prepare the sql script
    printf("\\pset tuples_only\n" \
           "\\pset format unaligned\n" \
           " -- invc - fixup\n" \
           "delete from svpf where svpf_patn__sequence = 0;\n" \
           "select     invc_fixfk(%d);\n" \
           " -- invc\n" \
           "select     %s\n" \
           " from      invc\n" \
           " where     invc__sequence = %d;\n" \
           " -- dbtr\n" \
           "select     %s\n" \
           " from      invc,dbtr\n" \
           " where     invc__sequence = %d\n" \
           " and       dbtr_code = invc_dbtr_code;\n" \
           " -- patn\n" \
           "select     %s\n" \
           " from      invc,patn\n" \
           " where     invc__sequence = %d\n" \
           " and       patn__sequence = invc_patn__sequence;\n" \
           " -- prov\n" \
           "select     %s\n" \
           " from      invc,prov\n" \
           " where     invc__sequence = %d\n" \
           " and       prov_code = invc_prov_code;\n" \
           " -- empl\n" \
           "select     %s\n" \
           " from      invc,empl\n" \
           " where     invc__sequence = %d\n" \
           " and       empl_code = invc_empl_code;\n" \
           " -- rfdr\n" \
           "select     %s\n" \
           " from      invc,rfdr\n" \
           " where     invc__sequence = %d\n" \
           " and       rfdr_code = invc_rfdr_code;\n" \
           " -- \n" \
           "select     %s\n" \
           " from      svpf\n" \
           " where     svpf_invc__sequence = %d;\n" \
           " -- invc\n" \
           "select     %s\n" \
           " from      crev\n" \
           " where     crev_invc__sequence = %d;\n" \
           "update     invc\n" \
           " set       invc_date_reprint = 'now'::timestamp\n" \
           " where     invc_date_printed is not null\n" \
           " and       invc__sequence = %d;\n" \
           "update     invc\n" \
           " set       invc_date_printed = 'now'::timestamp\n" \
           " where     invc_date_printed is null\n" \
           " and       invc__sequence = %d;\n",
           invc__sequence,
           invc_attlist,
           invc__sequence,
           dbtr_attlist,
           invc__sequence,
           patn_attlist,
           invc__sequence,
           prov_attlist,
           invc__sequence,
           empl_attlist,
           invc__sequence,
           rfdr_attlist,
           invc__sequence,
           svpf_attlist,
           invc__sequence,
           crev_attlist,
           invc__sequence,
           invc__sequence,
           invc__sequence) > tmpfile;
    close(tmpfile);
    
    psql =  sprintf("psql -q -d %s -f %s", dbname, tmpfile );

    count = 0;
    FS = "|";
    while ( psql | getline > 0 )
    {
        table = $1;
        table_count[table]++;
        sub(/^[^|]*\|/, "", $0);
        gsub(/<br>/,"");
        inputline[table,table_count[table]] = $0;
    }
    close(psql);

    exit; ## Jump to END{}
}

END     {

## Format the data
    initformat();
## Invoice headers
    split( inputline["invc", 1], invc_fields, "|" );

## The address
    
    split( inputline["dbtr", 1], dbtr_fields, "|" );
    split( inputline["patn", 1], patn_fields, "|" );
    printf "%s", vcurpos(44);
    printf "%s", font_tm_10;
    printf "%sAccount to:%sPatient: %s\n",
        hcurpos(10), hcurpos(100),
        patn_fields[patn["patn_flno"]];

    
    printf "%s%s", hcurpos(20), dbtr_fields[dbtr["dbtr_name"]];
    printf "%s%s %s %s\n", hcurpos(110), patn_fields[patn["patn_title"]],
        patn_fields[patn["patn_fsnam"]], patn_fields[patn["patn_psnam"]];
    printf "%s%s", hcurpos(20), dbtr_fields[dbtr["dbtr_address"]];
    printf "%s%s\n", hcurpos(110), patn_fields[patn["patn_address"]];
    printf "%s%s", hcurpos(20), dbtr_fields[dbtr["dbtr_suburb"]];
    printf "%s%s\n", hcurpos(110), patn_fields[patn["patn_suburb"]];
    printf "%s%s %s", hcurpos(20), dbtr_fields[dbtr["dbtr_state"]], dbtr_fields[dbtr["dbtr_postcode"]];
    printf "%s%s %s\n", hcurpos(110), patn_fields[patn["patn_state"]], patn_fields[patn["patn_postcode"]];
    if (patn_fields[patn["patn_healthcard"]] != "")
        printf "%sClaim Reference: %s\n", hcurpos(100), patn_fields[patn["patn_healthcard"]];
    if (patn_fields[patn["patn_dob"]] != "")
        printf "%sDOB: %s\n", hcurpos(100), patn_fields[patn["patn_dob"]];
    
## Services

    split( inputline["prov", 1], prov_fields, "|" );
    
    printf "%s%s", acurpos(10, 94), rectangle(175, 0.5, 90, 0);
    printf "%s%sFor Professional Services", acurpos(10, 100), font_tm_10b;
    printf "%s", vcurpos(110);
    printf "%s%s%s %s\n\n", hcurpos(15), font_tm_10b,
        prov_fields[prov["prov_name"]],
        prov_fields[prov["prov_provider_num"]];
    printf "%s", font_tm_10;

    for ( i_svpf = 1; i_svpf <= table_count["svpf"]; i_svpf++ )
    {
        split( inputline["svpf", i_svpf], svpf_fields, "|" );
# Date
        printf "%s%s%s", hcurpos(20), font_tm_10,
            svpf_fields[svpf["svpf_date_service"]];
# Item
        printf "%s%s", hcurpos(40),
            svpf_fields[svpf["svpf_serv_code"]]; 
# Description
        printf "%s%s", hcurpos(55),
            svpf_fields[svpf["svpf_desc"]]; 
# Amount
        printf "%s%s%10.2f", hcurpos(140), font_fx_10b,
            svpf_fields[svpf["svpf_amount"]];
# GST
        printf "%s%s%10.2f\n", hcurpos(170), font_fx_10b,
            svpf_fields[svpf["svpf_gst_amount"]];
    }

## Referral    
    split( inputline["rfdr", 1], rfdr_fields, "|" );
    printf "\n%s%sReferred by:", hcurpos(20), font_tm_10;
    printf "%s", font_tm_10;
    printf "%s%s\n", hcurpos(45),
        rfdr_fields[rfdr["rfdr_name"]];
    printf "%s%s\n", hcurpos(45),
        rfdr_fields[rfdr["rfdr_street"]];
    printf "%s%s %s\n", hcurpos(45),
        rfdr_fields[rfdr["rfdr_suburb"]],
        rfdr_fields[rfdr["rfdr_postcode"]];
    printf "%s%s\n", hcurpos(45),
        invc_fields[invc["invc_rfdr_date"]];
    period = invc_fields[invc["invc_rfdr_period"]] + 0;
    period_str = sprintf("%d months", period);
    if ( period == 0 ) period_str = "";
    if ( period == 99) period_str = "Indefinite";
    printf "%s%s\n", hcurpos(45),
        period_str;
    printf "\n%s%sHealth Fund:", hcurpos(20), font_tm_10;
    printf "%s", font_tm_10;
    printf "%s%s\n", hcurpos(45),
        patn_fields[patn["patn_hlfd_code"]];
    printf "\n%s%sMedicare #:", hcurpos(20), font_tm_10;
    printf "%s", font_tm_10;
    printf "%s%s\n", hcurpos(45),
        patn_fields[patn["patn_medicare"]];

########################################
# Summary
########################################

    printf "%s%sInvoice Summary", acurpos(80, 200), font_tm_10b;
    printf "%s%s", acurpos(80, 202), rectangle(81, 0.5, 100, 0);
    printf "%s\n", font_tm_10;

## Total fees
    
    printf "%sTotal Fees%s%s%10.2f\n", hcurpos(80), hcurpos(140), font_fx_10b,
        invc_fields[invc["invc_amount"]];
    printf "%sGST%s%s%10.2f\n", hcurpos(80), hcurpos(140), font_fx_10b,
        invc_fields[invc["invc_gst_amount"]];
    printf "%s%sLess payments received:\n", hcurpos(80), font_tm_10;

## Credits

    for (i_crev = 1; i_crev <= table_count["crev"]; i_crev++ )
    {
        split( inputline["crev", i_crev], crev_fields, "|" );
# Receipt number
        printf "%s%s%s", hcurpos(90), font_tm_10,
            crev_fields[crev["crev__sequence"]];
# Receipt date
        printf "%s%s", hcurpos(110),
            crev_fields[crev["crev_date_entry"]];
# Receipt amount
        printf "%s%s%10.2f\n", hcurpos(140), font_fx_10b,
            crev_fields[crev["crev_cred_amount"]] + crev_fields[crev["crev_cred_gst_amount"]];
    }

## Total-balance
    
    printf "\n";
    printf "%s%s\n", hcurpos(140), rectangle(23, 7, 20, 2);
    printf "%s%sAMOUNT OWING%s%s%10.2f\n", hcurpos(100), font_tm_10b, hcurpos(140), font_fx_10b,
        invc_fields[invc["invc_amount"]] \
        + invc_fields[invc["invc_gst_amount"]] \
        - invc_fields[invc["invc_paid_amount"]] \
        - invc_fields[invc["invc_paid_gst_amount"]];
    printf "%s%sINVOICE #%s", hcurpos(165), font_tm_8,
        invc__sequence;
    printf "\f";
}

