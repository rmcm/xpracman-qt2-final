--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 


-- ------------------------------------------------------------
-- Function: Create a batch (session) from an invoice
-- ------------------------------------------------------------

drop function fn_create_batch(integer);
create function fn_create_batch(integer)
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;

        x_invc__sequence ALIAS for $1;
        tmp_invc invc%ROWTYPE;
        tmp_svpf_count integer;
        tmp_mdaf__sequence mdaf.mdaf__sequence%TYPE;
        tmp_mdbt__sequence mdbt.mdbt__sequence%TYPE;

    BEGIN

        -- check for the invoice.

        select  *
        into    tmp_invc
        from    invc
        where   invc__sequence = x_invc__sequence;

        if (not found) then
          RAISE EXCEPTION ''Invoice number % not found'', x_invc__sequence;
          return null;
        end if;

        -- check that the invoice has not been printed

        if (tmp_invc.invc_date_printed is not null) then
          RAISE EXCEPTION ''Invoice has been printed already, so the included services cannot be Bulk-Billed'';
          return null;
        end if;

        -- count the unclaimed services

        select  count(*)
        into    tmp_svpf_count
        from    svpf
        where   svpf_invc__sequence = x_invc__sequence
        and     svpf_mdaf__sequence is null;

        if (tmp_svpf_count < 1) then
          RAISE EXCEPTION ''There are no services to be claimed for this invoice - % '', x_invc__sequence;
          return null;
        end if;

        -- create a voucher

        select  nextval(''mdaf_mdaf__sequence_seq'')
        into    tmp_mdaf__sequence;
        insert into mdaf(mdaf__sequence, mdaf_patn__sequence, mdaf_prov_code, mdaf_rfdr_code)
                values(tmp_mdaf__sequence, tmp_invc.invc_patn__sequence, tmp_invc.invc_prov_code, tmp_invc.invc_rfdr_code);
        update  svpf
        set     svpf_mdaf__sequence = tmp_mdaf__sequence
        where   svpf_invc__sequence = x_invc__sequence
        and     svpf_mdaf__sequence is null;

        -- find an open batch

        select  mdbt__sequence
        into    tmp_mdbt__sequence
        from    mdbt
        where   mdbt_prov_code = tmp_invc.invc_prov_code
        and     mdbt_claim_date is null;

        if (not found) then
          -- create a batch
  
          select  nextval(''mdbt_mdbt__sequence_seq'')
          into    tmp_mdbt__sequence;
  
          insert  into mdbt(mdbt_prov_code, mdbt__sequence, mdbt_voucher_count, mdbt_total_amount)
          select  tmp_invc.invc_prov_code,
                  tmp_mdbt__sequence,
                  1,
                  invc_balance(x_invc__sequence);
        end if;
  
        update  mdaf
        set     mdaf_mdbt__sequence = tmp_mdbt__sequence
        where   mdaf__sequence = tmp_mdaf__sequence;

        -- close the invoice
        update  invc
        set     invc_date_printed = now()
        where   invc__sequence = x_invc__sequence;

    return tmp_mdaf__sequence;

    END;'
  LANGUAGE 'plpgsql';

-- update history;
\set mttb_name '\'mdbt\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
