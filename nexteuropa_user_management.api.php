<?php

/**
 * @file
 * APIs for nexteuropa_user_management module.
 */

/**
 * Allow modules to grant permissions for UM Administrator.
 *
 * @return array
 *   An array of permissions that you wish to add to UM Administrator, these
 *   roles will be filtered.
 */
function hook_um_administrator_grant_permissions() {
  return array(
    'create files',
    'create messages',
    'create new books',
    'create translation jobs',
    'create url aliases',
    /* ... */
  );
}
