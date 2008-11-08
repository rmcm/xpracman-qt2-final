#!/usr/bin/awk -f
#
#     $Id: bank.awk 1366 2002-06-06 04:37:24Z source $
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
# usage: env DBNAME=prac bkdv__sequence=5002 invoice.awk 

BEGIN {
    dbname = ENVIRON["DBNAME"];
    reconciliation = ENVIRON["reconciliation"];
    brief_reconciliation = ENVIRON["BRIEF_RECONCILIATION"];
    bkdv_bkdp__sequence = ENVIRON["bkdv_bkdp__sequence"];
    if ( dbname == "" || bkdv_bkdp__sequence == "")
        exit 1;
    bank_sql(dbname, bkdv_bkdp__sequence);
    if ( reconciliation == "" )
    {
        if ( bkdv_bkdp__sequence == 0 )
            reconciliation = 1;
        else
            reconciliation = 0;

    }
    if ( brief_reconciliation && reconciliation == 1 )
        reconciliation = 2;
    if ( reconciliation > 0 )
            page_title = "RECEIPTS RECONCILIATION REPORT";
    else
            page_title = "B A N K   D E P O S I T   R E P O R T"

    exit; ## Jump to END{}
}

function heading(text) {
    pages++;
    printf "%s%s%s : %s\n",
        acurpos(10, 20),
        font_tm_10b,
        page_title,
        text;
    printf "%s%s\n",
        hcurpos(10),
        rectangle(175, 0.5, 90, 0);
    printf "%s%sDrawer%sBank%sBranch%sAmount\n",
        font_tm_10b,
        hcurpos(50),
        hcurpos(100),
        hcurpos(130),
        hcurpos(170);
    printf "%s%s\n",
        hcurpos(10),
        rectangle(175, 0.5, 90, 0);
    
}

function footing() {
    printf "%s%s\n",
        acurpos(10,270),
        rectangle(175, 0.5, 90, 0);
    printf "%s%s Page %d: Batch # %d: %s\n",
        hcurpos(10),
        font_fx_10b,
        pages,
        bkdv_bkdp__sequence,
        strftime("%c");
    printf "\f";
}

function banktotal() {
## Bank total - new page    
}

function listneeds() {
## check remaining space;
    need = 10; ## fixed overhead of newlines
    for ( subt in total_subt )
        need++;
    return needs;
}

function listtotal() {
## list totals - new page
    printf "\n\n%s%sL I S T   S U M M A R Y \n",
        hcurpos(90),
        font_tm_10b;
    printf "\n\n%s%sTotals by Tender-type: \n",
        hcurpos(90),
        font_tm_10b;
    printf "%s%s\n",
        hcurpos(90),
        rectangle(95, 0.5, 90, 0);

## Summary
    for ( subt in total_subt )
    {
        split(subt, listarray, SUBSEP);
        if ( listarray[1] == curr_bank && listarray[2] == curr_list )
        {
            printf "%s%s%s:%s%s%10.2f\n",
                hcurpos(90),
                font_tm_10,
                listarray[3],
                hcurpos(150),
                font_fx_10b,
                total_subt[subt];
        }
    }
    id_list_total = sprintf("%s\034%s", curr_bank, curr_list);
    printf "\n\n%s%s%s:%s%s%10.2f\n",
        hcurpos(90),
        font_tm_10b,
        "List Total",
        hcurpos(150),
        font_fx_10b,
        total_list[id_list_total];
    printf "%s%s\n",
        hcurpos(90),
        rectangle(95, 0.5, 90, 0);

}

function subtotal() {
## Subtotals  

}

function grandtotal() {
## top-level totals
}

END     {

## Format the data

    initformat();
    printf "%s%s", init_pr, font_fx_10;
    
## The list is sorted by bkdv_bank_code,bkdv_tdtp_list,bkdv_tdtp_subtotal,bkdv_tdtp_code
## Payments are grouped into lists - by tdtp_list
## New lists start on new pages.
## Within bkdv_bank_code, payments are totalled by;
##      - tender type (tdtp_code)
##      - list type   (tdtp_list)
##      - subtotal type (tdtp_subtotal)
    
    curr_bkdv_bank_code = "XXX";
    curr_bkdv_tdtp_list = "XXX";
    curr_bkdv_tdtp_subtotal = "XXX";
    
    curr_paym = -1;
    curr_cred = -1;
    curr_invc = -1;
    
    total_bkdv = 0.0;
    lines = 1;          # line count on page
    linespp = 48;       # max lines on page for detail
    pages = 0;          # Page count
    for ( i_bkdv = 1; i_bkdv <= table_count["bkdv"]; i_bkdv++ )
    {
        
## Unpack the data
        split( inputline["bkdv", i_bkdv], bkdv_fields, "|" );
        tdtp_desc[bkdv_fields[bkdv["bkdv_tdtp_code"]]] = bkdv_fields[bkdv["tdtp_desc"]];

        if ( i_bkdv == 1 )      ## First page; initialise blocks
        {
            curr_bank = bkdv_fields[bkdv["bkdv_bank_code"]];
            curr_list = bkdv_fields[bkdv["bkdv_tdtp_list"]];
            curr_subt = bkdv_fields[bkdv["bkdv_tdtp_subtotal"]];
            curr_tdtp = "";
        }

        if ( curr_bank != bkdv_fields[bkdv["bkdv_bank_code"]] || pages == 0)          # New bank
        {
            if ( pages > 0)
            {
                banktotal();
                footing();
            }
            curr_bank = bkdv_fields[bkdv["bkdv_bank_code"]];
            curr_bank_text = sprintf("%s - (%s %s) %s",
                                     bkdv_fields[bkdv["bkdv_bank_name"]],
                                     bkdv_fields[bkdv["bkdv_bank_bank"]],
                                     bkdv_fields[bkdv["bkdv_bank_account"]],
                                     curr_list);
            heading(curr_bank_text);
            lines = 1;
        }
        if ( curr_list != bkdv_fields[bkdv["bkdv_tdtp_list"]] )          # New list
        {
            if ( pages > 0)
            {
                if ( (listneeds() + lines) > linespp )
                {
                    footing();
                    heading(curr_bank_text);
                    lines = 1;
                }
                listtotal();
                footing();
                lines = 1;
            }
            curr_list = bkdv_fields[bkdv["bkdv_tdtp_list"]];
            curr_bank_text = sprintf("%s (%s %s) %s",
                                     bkdv_fields[bkdv["bkdv_bank_name"]],
                                     bkdv_fields[bkdv["bkdv_bank_bank"]],
                                     bkdv_fields[bkdv["bkdv_bank_account"]],
                                     curr_list);
            heading(curr_bank_text);
            lines = 1;
        }
        if ( curr_subt != bkdv_fields[bkdv["bkdv_tdtp_subtotal"]] )  # New subtotal
        {
            subtotal();
            curr_subt = bkdv_fields[bkdv["bkdv_tdtp_subtotal"]];
        }
        
        if ( curr_tdtp != bkdv_fields[bkdv["bkdv_tdtp_code"]] )  # New t-type heading
        {
            curr_tdtp = bkdv_fields[bkdv["bkdv_tdtp_code"]];
            printf "\n%s%s%s\n\n",
                hcurpos(10),
                font_tm_10b,
                bkdv_fields[bkdv["tdtp_desc"]];
            lines = lines + 3;
        }
            
        if ( (listneeds() + lines) > linespp && pages >= 1 )  ## New page
        {
            footing();
            heading(curr_bank_text);
            lines = 1;
        }
## New payment
        if ( curr_paym != bkdv_fields[bkdv["bkdv_paym__sequence"]] )
        {
            multi = 0; ## default single patient payment
            curr_paym = bkdv_fields[bkdv["bkdv_paym__sequence"]];
            if ( reconciliation == 2 )
            {
                printf "%s%s%-10.10s",
                    hcurpos(0),
                    font_tm_10,
                    bkdv_fields[bkdv["bkdv_date_entry"]];
## check for multi-patient cheques
                if ( bkdv_fields[bkdv["bkdv_paym_amount"]]+0.0 \
                     == bkdv_fields[bkdv["bkdv_cred_amount"]]+0.0 )
                {
                    patlabel = substr(bkdv_fields[bkdv["bkdv_patn_psnam"]],1,10);
                    multi = 0;
                } else {
                    patlabel = "Mulitple";
                    multi = 1;
                }
                printf "%s%-10.10s",
                    hcurpos(20),
                    patlabel;
            }
            printf "%s%s%s",
                hcurpos(50),
                font_tm_10,
                bkdv_fields[bkdv["bkdv_drawer"]];
            printf "%s%s%s",
                hcurpos(100),
                font_tm_10,
                bkdv_fields[bkdv["bkdv_bank"]];
            printf "%s%s%s",
                hcurpos(130),
                font_tm_10,
                bkdv_fields[bkdv["bkdv_branch"]];
            amount = bkdv_fields[bkdv["bkdv_paym_amount"]]+0.0;
            printf "%s%s%10.2f\n",
                hcurpos(160),
                font_fx_10,
                amount;
            total_bank[curr_bank] += amount;
            total_list[curr_bank,curr_list] += amount;
            total_subt[curr_bank,curr_list,curr_subt] += amount;
            total_bkdv += amount;
            tdtp = bkdv_fields[bkdv["bkdv_tdtp_code"]];
            total_tdtp[curr_bank,curr_list,curr_subt,tdtp] += amount;
            lines++;
        }
        if ( reconciliation == 1 )
        {
            credit_amount = bkdv_fields[bkdv["bkdv_cred_amount"]] \
                +bkdv_fields[bkdv["bkdv_cred_gst_amount"]];
            printf "%s%s%s%s%s(%s)%sInv# %s%s%s%10.2f\n",
                hcurpos(20),
                font_tm_10,
		substr(bkdv_fields[bkdv["bkdv_date_entry"]],1,10),
                hcurpos(80),
                bkdv_fields[bkdv["bkdv_patn_psnam"]],
                bkdv_fields[bkdv["bkdv_patn__sequence"]],
                hcurpos(130),
                bkdv_fields[bkdv["bkdv_invc__sequence"]],
                hcurpos(145),
                font_fx_10,
                credit_amount;
            lines++;
            printf "%s%s\n",
                hcurpos(80),
                rectangle(90, 0.5, 90, 0);
            lines++;
            
        }
## List if multiple patient and brief reconciliation
        if ( reconciliation == 2 && multi == 1 )
        {
            credit_amount = bkdv_fields[bkdv["bkdv_cred_amount"]] \
                +bkdv_fields[bkdv["bkdv_cred_gst_amount"]];
            printf "%s%s%s%s%10.2f\n",
                hcurpos(80),
                substr(bkdv_fields[bkdv["bkdv_patn_psnam"]],1,10),
                hcurpos(125),
                font_fx_10,
                credit_amount;
            lines++;  
        }

    }
## Cleanup

    subtotal();
    if ( (listneeds() + lines) > linespp )
    {
        footing();
        heading(curr_bank_text);
        lines = 1;
    }
    listtotal();
    banktotal();
    
## Last page summaries
    footing();
    heading(curr_bank_text);
    lines = 1;
    
    printf "\n\n%s%sD E P O S I T S   S U M M A R Y \n",
        hcurpos(90),
        font_tm_10b;
    printf "\n\n%s%sTotals by Tender-type: \n",
        hcurpos(90),
        font_tm_10b;
    printf "%s%s\n",
        hcurpos(90),
        rectangle(95, 0.5, 90, 0);

## Summary
    for ( tt in total_tdtp)
    {
        split(tt, tt_keys, SUBSEP);
        curr_bank = tt_keys[1];
        curr_list = tt_keys[2];
        curr_subt = tt_keys[3];
        tdtp      = tt_keys[4];
        
        printf "%s%s%s:%s%s%10.2f\n",
            hcurpos(90),
            font_tm_10,
            tdtp_desc[tdtp],
            hcurpos(150),
            font_fx_10,
            total_tdtp[tt];
    }
    printf "\n\n%s%s%s:%s%s%10.2f\n",
        hcurpos(90),
        font_tm_10,
        "Site Total",
        hcurpos(150),
        font_fx_10,
        total_bkdv;
    printf "%s%s\n",
        hcurpos(90),
        rectangle(95, 0.5, 90, 0);

    footing();
}

