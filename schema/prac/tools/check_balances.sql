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
select  'Checking INVC,SVPF and CRED Balances' as title;
-- Service totals
drop table invc_svpf_totals;
select  svpf_invc__sequence, 
        sum(svpf_amount) as svpf_amount,
        sum(svpf_gst_amount) as svpf_gst_amount
into    invc_svpf_totals
from    svpf
group by svpf_invc__sequence;


-- Credit totals
drop table invc_cred_totals;
select  cred_invc__sequence, 
        sum(cred_amount) as cred_amount, 
        sum(cred_gst_amount) as cred_gst_amount
into    invc_cred_totals
from    cred 
group by cred_invc__sequence;

-- Payment totals
drop table paym_cred_totals;
select  cred_paym__sequence, 
        sum(cred_amount) as cred_amount, 
        sum(cred_gst_amount) as cred_gst_amount
into    paym_cred_totals
from    cred 
group by cred_paym__sequence;

-- Check against static balances
select  'Comparing SVPF to INVC - looking for conflict' as title;
select  invc__sequence,
        invc_amount,
        invc_gst_amount,
        svpf_amount,
        svpf_gst_amount
from    invc LEFT JOIN invc_svpf_totals 
ON      (invc.invc__sequence = invc_svpf_totals.svpf_invc__sequence)
where   invc_amount != svpf_amount
or      invc_gst_amount != svpf_gst_amount;
        
select  'Comparing CRED to INVC - looking for conflict' as title;
select  invc__sequence,
        invc_paid_amount,
        invc_paid_gst_amount,
        cred_amount,
        cred_gst_amount
from    invc LEFT JOIN invc_cred_totals 
ON      (invc.invc__sequence = invc_cred_totals.cred_invc__sequence)
where   invc_paid_amount != cred_amount
or      invc_paid_gst_amount != cred_gst_amount;
        
select  'Comparing PAYM to CRED - looking for conflict' as title;
select  paym__sequence,
        paym_amount,
        coalesce(cred_amount, 0.00) + coalesce(cred_gst_amount, 0.00),
        cred_amount,
        cred_gst_amount
from    paym LEFT JOIN paym_cred_totals
ON      (paym.paym__sequence = paym_cred_totals.cred_paym__sequence)
where   paym_amount != coalesce(cred_amount, 0.00) + coalesce(cred_gst_amount, 0.00);
