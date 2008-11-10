--  -*- sql -*-
--  
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

-- 
-- Create a function to insert via view
-- This function returns the serial_number of the new "virtual" row.
-- This is used instead of an "insert" rule, as the latter cannot use "lastoid"
-- to revover the inserted row.
-- 

create or replace function crev_insert_row()
returns integer
as '
    DECLARE
                rec_crev crev%ROWTYPE;
                tmp_paym__sequence paym.paym__sequence%TYPE; 
                tmp_cred__sequence cred.cred__sequence%TYPE; 
                rec_invc invc%ROWTYPE;
                rec_cred cred%ROWTYPE;
                rec_paym paym%ROWTYPE;
                
    BEGIN

         -- Do not know invoice yet, so use 0 
         if ( rec_crev.crev_invc__sequence is null) then
           -- new invoice = 0 to trigger
           rec_crev.crev_invc__sequence := 0;
           insert into invc(invc__sequence) select 0 except select invc__sequence from invc;
           insert into patn(patn__sequence) select 0 except select patn__sequence from patn;
         end if;

         -- set defaults
         if ( rec_crev.crev_date_entry is null ) then
            rec_crev.crev_date_entry := now();
         end if;

         if ( rec_crev.crev_user_entry is null ) then
            rec_crev.crev_user_entry := getpgusername();
         end if;

         if ( rec_crev.crev_site_entry is null ) then
            rec_crev.crev_site_entry := ''-'';
         end if;

            -- set to non-null
         if ( rec_crev.crev_paym_amount is null ) then
            rec_crev.crev_paym_amount := 0.0;
         end if;

         if ( rec_crev.crev_tdtp_code is null ) then
            rec_crev.crev_tdtp_code := ''-'';
         end if;

         -- set to non-null
         if ( rec_crev.crev_cred_amount is null ) then
            rec_crev.crev_cred_amount := 0.0;
         end if;

         -- set to non-null
         if ( rec_crev.crev_cred_gst_amount is null ) then
            rec_crev.crev_cred_gst_amount := 0.0;
         end if;

         -- get a paym__sequence
         select nextval(''paym_paym__sequence_seq'')
         into tmp_paym__sequence;


        --
        -- Debugging
        --
        -- RAISE NOTICE ''OUT->%/%/%/%/%/%/%/%/%/%/%/%/'',
        --              rec_crev.crev_date_entry,
        --              rec_crev.crev_user_entry,
        --              rec_crev.crev_site_entry,
        --              rec_crev.crev_paym_amount,
        --              rec_crev.crev_tdtp_code,
        --              rec_crev.crev_drawer,
        --              rec_crev.crev_bank,
        --              rec_crev.crev_branch,
        --              tmp_paym__sequence,
        --              rec_crev.crev_invc__sequence,
        --              rec_crev.crev_cred_amount,
        --              rec_crev.crev_cred_gst_amount;

         -- add the paym record
         insert into paym(
                     paym_date_entry,
                     paym_user_entry,
                     paym_site_entry,
                     paym_amount,
                     paym_tdtp_code,
                     paym_drawer,
                     paym_bank,
                     paym_branch,
                     paym__sequence)
              values(
                     rec_crev.crev_date_entry,
                     rec_crev.crev_user_entry,
                     rec_crev.crev_site_entry,
                     rec_crev.crev_paym_amount,
                     rec_crev.crev_tdtp_code,
                     rec_crev.crev_drawer,
                     rec_crev.crev_bank,
                     rec_crev.crev_branch,
                     tmp_paym__sequence);

         -- get a cred__sequence
         select nextval(''cred_cred__sequence_seq'')
         into tmp_cred__sequence;
         -- add the cred record 
         insert into cred(
                     cred_paym__sequence,
                     cred_invc__sequence,
                     cred_amount,
                     cred_gst_amount,
                     cred__sequence)
               values(
                     tmp_paym__sequence,
                     rec_crev.crev_invc__sequence,
                     rec_crev.crev_cred_amount,
                     rec_crev.crev_cred_gst_amount,
                     tmp_cred__sequence);

    return tmp_cred__sequence;
    END;'
    LANGUAGE 'plpgsql';



-- update history;
\set mttb_name '\'crev\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
