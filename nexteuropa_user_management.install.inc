<?php

/**
 * @file
 * Helper functions for install file.
 */

/**
 * This function creates the UM Administrator and assign some permissions.
 *
 * The permissions that is assigned is the ones that will the cron job check on
 * every run.
 * This function is used at install and on update too.
 *
 * @throws \InvalidMergeQueryException
 *   Thrown if db_merge fails for some reason.
 *
 * @see nexteuropa_user_management_install()
 * @see nexteuropa_user_management_update_7001()
 */
function _nexteuropa_user_management_add_um_administrator_role() {
  // Ensure translations don't break during installation.
  $t = get_t();

  $role = new stdClass();
  $role->name = NEXTEUROPA_USER_MANAGEMENT_UM_ADMINISTRATOR_ROLE;
  if (user_role_save($role) === FALSE) {
    drupal_set_message($t('%role role creation failed!', array(
      '%role' => NEXTEUROPA_USER_MANAGEMENT_UM_ADMINISTRATOR_ROLE,
    )), 'error');
    return;
  }

  variable_set('nexteuropa_user_management_um_administrator_rid', $role->rid);
  _nexteuropa_user_management_check_um_administrator_permissions();

  // In case of install the permission is still unknown by drupal, so we need to
  // add it manually.
  db_merge('role_permission')
    ->key(array(
      'rid' => $role->rid,
      'permission' => 'nexteuropa manage users purge cache',
      'module' => 'nexteuropa_user_management',
    ))
    ->execute();
  db_merge('role_permission')
    ->key(array(
      'rid' => $role->rid,
      'permission' => 'administer nexteuropa user management',
      'module' => 'nexteuropa_user_management',
    ))
    ->execute();
  db_merge('role_permission')
    ->key(array(
      'rid' => variable_get('nexteuropa_user_management_user_manager_rid'),
      'permission' => 'administer nexteuropa user management',
      'module' => 'nexteuropa_user_management',
    ))
    ->execute();
}
