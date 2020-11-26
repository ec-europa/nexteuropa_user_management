<?php

/**
 * @file
 * This file contains some of the pre defined permissions.
 */

/**
 * This functions defines the forbidden permissions.
 *
 * These permissions shouldn't be given in any circumstances to any role.
 *
 * @return string[]
 *   Forbidden permissions.
 */
function _nexteuropa_user_management_get_forbidden_permissions() {
  $forbidden = array(
    'administer content types',
    'access all views',
    'administer ckeditor_lite',
    'administer ecas',
    'administer features',
    'administer fields',
    'administer file types',
    'administer filters',
    'administer jquery update',
    'administer modules',
    'administer om maximenu',
    'administer page manager',
    'administer permissions',
    'administer site configuration',
    'administer software updates',
    'administer themes',
    'administer users',
    'administer views',
    'bypass file access',
    'bypass node access',
    'bypass rules access',
    'generate features',
    'manage feature nexteuropa_dgt_connector',
    'manage features',
    'rename features',
    'use page manager',
  );
  $forbidden = array_unique(array_merge($forbidden, module_invoke_all('user_block_permissions')));
  return $forbidden;
}

/**
 * This functions defines the 'do not give' permissions.
 *
 * These permissions are not good idea to give on production sites due possible
 * config changes or some other security reasons. You can find it in the
 * 'NEPT Admin_permissions.xls' file attached to the NEPT-2830 ticket.
 *
 * These permissions are not possible to give directly to UM Administrator role.
 *
 * @return string[]
 *   Do not give permissions.
 */
function _nexteuropa_user_management_get_do_not_give_permissions() {
  $do_not_give_permissions = &drupal_static(__FUNCTION__);

  if (!isset($do_not_give_permissions)) {
    // First set static permissions.
    $do_not_give_permissions = array(
      'access bean overview',
      'access printer-friendly version',
      'access rules debug',
      'access site in maintenance mode',
      'access social bookmark config page',
      'admin_classes',
      'admin_display_suite',
      'admin_fields',
      'admin_view_modes',
      'administer actions',
      'administer autologout',
      'administer backends',
      'administer bean settings',
      'administer bean types',
      'administer beans',
      'administer blocks',
      'administer book outlines',
      'administer CAPTCHA settings',
      'administer chr',
      'administer ckeditor link',
      'administer coffee',
      'administer comments',
      'administer consumers',
      'administer contact forms',
      'administer contexts',
      'administer custom content',
      'administer date tools',
      'administer dgt connector',
      'administer drafty',
      'administer easy_breadcrumb',
      'administer email non-delivery report processing',
      'administer entity translation',
      'administer EU Cookie Compliance popup',
      'administer facets',
      'administer feature sets',
      'administer feeds',
      'administer feeds_tamper',
      'administer fieldgroups',
      'administer files',
      'administer flags',
      'administer flexslider',
      'administer forums',
      'administer frontend cache purge rules',
      'administer group',
      'administer image styles',
      'administer languages',
      'administer linkchecker',
      'administer mail templates',
      'administer mailsystem',
      'administer media browser',
      'administer menu_token',
      'administer message types',
      'administer meta tags',
      'administer news feeds',
      'administer newsletter statistics',
      'administer newsletters',
      'administer newsroom advanced settings',
      'administer newsroom settings',
      'administer nexteuropa cookie consent kit configuration',
      'administer nexteuropa laco settings',
      'administer nexteuropa_piwik',
      'administer nodes',
      'administer password policies',
      'administer password policy',
      'administer pathauto',
      'administer print',
      'administer producers',
      'administer quicktabs',
      'administer realname',
      'administer redirects',
      'administer registration',
      'administer registration states',
      'administer registration types',
      'administer rules',
      'administer scheduler',
      'administer simplenews settings',
      'administer statistics',
      'administer taxonomy',
      'administer taxonomy_browser',
      'administer tmgmt',
      'administer translation tasks',
      'administer unit tests',
      'administer user subscriptions',
      'administer uuid',
      'administer video presets',
      'administer voting api',
      'administer webtools',
      'administer workbench',
      'administer workbench moderation',
      'administer xmlsitemap',
      'advanced migration information',
      'block IP addresses',
      'bulk-administer user subscriptions',
      'bypass advanced aggregation',
      'bypass conversion video',
      'bypass maxlength',
      'bypass workbench moderation',
      'change own logout threshold',
      'change own username',
      'configure flickr',
      'configure poetry client',
      'context ajax block access',
      'convert on submission',
      'display drupal links',
      'edit bean view mode',
      'edit link settings',
      'edit mimemail user settings',
      'force password change',
      'import media',
      'manage feature dgt connector',
      'migration information',
      'node-specific print configuration',
      'notify of path changes',
      'og subscribe',
      'select account cancellation method',
      'send arbitrary files',
      'send unlimited emails',
      'skip CAPTCHA',
      'toggle field translatability',
      'translate admin strings',
      'translate user-defined strings',
      'unblock expired accounts',
      'upload webtools custom js',
      'use ctools import',
      'use flag import',
      'use PHP for tracking visibility',
      'version management edits',
      'view private files',
    );

    // Set dynamically generated permissions.
    if (module_exists('bean')) {
      bean_reset(TRUE);
      foreach (bean_get_types() as $bean_type) {
        $bean_type_name = check_plain($bean_type->type);
        $do_not_give_permissions[] = "create any $bean_type_name bean";
        $do_not_give_permissions[] = "edit any $bean_type_name bean";
        $do_not_give_permissions[] = "delete any $bean_type_name bean";
      }
    }
    if (module_exists('ds_extras')) {
      foreach (node_type_get_names() as $key => $name) {
        $do_not_give_permissions[] = 'ds_switch ' . $key;
      }
    }
    if (module_exists('feeds')) {
      foreach (feeds_importer_load_all() as $importer) {
        $do_not_give_permissions[] = "clear $importer->id feeds";
      }

      // This one has a dependency on feeds, I've included the original check
      // too just for safety. The following is the original comment:
      // For whatever reason, this gets called withtout the Feeds module being
      // loaded in certain odd circumstances.
      if (function_exists('feeds_importer_load_all') && module_exists('feeds_tamper')) {
        foreach (feeds_importer_load_all() as $importer) {
          $do_not_give_permissions[] = 'tamper ' . $importer->id;
        }
      }
    }
    if (module_exists('multisite_drupal_features_set_standard')) {
      $featuresets = feature_set_get_featuresets();
      foreach ($featuresets as $featureset => $details) {
        $do_not_give_permissions[] = 'manage feature ' . $featureset;
      }
    }
    if (module_exists('registration')) {
      foreach (registration_get_types() as $type_info) {
        $do_not_give_permissions[] = "administer $type_info->name registration";
        $do_not_give_permissions[] = "administer own $type_info->name registration";
        $do_not_give_permissions[] = "create $type_info->name registration";
        $do_not_give_permissions[] = "create $type_info->name registration self";
      }
    }
  }

  return $do_not_give_permissions;
}

/**
 * This functions defines the 'suggested' or 'can be assigned' permissions.
 *
 * These permissions are suggested to give on production sites. You can find it
 * in the 'NEPT Admin_permissions.xls' file attached to the NEPT-2830 ticket.
 *
 * @return string[]
 *   Suggested permissions.
 */
function _nexteuropa_user_management_get_suggested_permissions() {
  $suggested = &drupal_static(__FUNCTION__);

  if (!isset($suggested)) {
    $suggested = array(
      'accept translation jobs',
      'access administration menu',
      'access administration pages',
      'access broken links report',
      'access ckeditor link',
      'access coffee',
      'access comments',
      'access content overview',
      'access content',
      'access contextual links',
      'access dashboard',
      'access ecas import users function',
      'access media browser',
      'access news feeds',
      'access own broken links report',
      'access PDF version',
      'access print',
      'access private area',
      'access send by email',
      'access site map',
      'access site reports',
      'access site-wide contact form',
      'access statistics',
      'access to the dgt connector cart',
      'Access to unpublished file',
      'access user contact forms',
      'access user profiles',
      'access workbench',
      'add content to books',
      'add media from remote sources',
      'administer content translations',
      'administer menu attributes',
      'administer menu',
      'administer nexteuropa user management',
      'administer search',
      'administer search_api',
      'administer simplenews subscriptions',
      'administer splash screen',
      'administer url aliases',
      'cancel account',
      'cancel own vote',
      'ckeditor_lite highlight changes',
      'create files',
      'create messages',
      'create new books',
      'create translation jobs',
      'create url aliases',
      'delete revisions',
      'diff view changes',
      'display EU Cookie Compliance popup',
      'edit meta tags',
      'edit original values',
      'edit own comments',
      'edit remote newsroom item',
      'edit translation shared fields',
      'flush caches',
      'import newsroom feeds',
      'inspect all votes',
      'moderate forums',
      'multisite_workbench_moderation_view_bulk_update',
      'nexteuropa manage users purge cache',
      'opt-in or out of tracking',
      'override default scheduler time',
      'override player dimensions',
      'post comments',
      'provide translation services',
      're convert video',
      'revert revisions',
      'schedule (un)publishing of nodes',
      'schedule publishing of nodes',
      'scheduler workbench',
      'search content',
      'send newsletter',
      'send newsroom item proposal',
      'show format tips',
      'show more format tips link',
      'skip comment approval',
      'submit translation jobs',
      'subscribe to all content types',
      'subscribe to content types',
      'subscribe to content',
      'subscribe to newsletters',
      'subscribe to taxonomy terms',
      'suspend own subscriptions',
      'translate any entity',
      'translate content',
      'translate interface',
      'update any fullcalendar event',
      'use advanced search',
      'use default thumb',
      'use media wysiwyg',
      'use workbench_moderation my drafts tab',
      'use workbench_moderation needs review tab',
      'use xmlsitemap',
      'view advanced help index',
      'view advanced help popup',
      'view advanced help topic',
      'view alert message',
      'view all unpublished content',
      'view bean page',
      'view bean revisions',
      'view date repeats',
      'view files',
      'view mimemail user settings',
      'view moderation history',
      'view moderation messages',
      'view newsletter statistics',
      'view own files',
      'view own private files',
      'view own unpublished content',
      'view post access counter',
      'view rate results page',
      'view revisions',
      'view scheduled content',
      'view the administration theme',
      'vote on polls',
    );

    // Check for dynamically generated ones.
    if (module_exists('bean')) {
      // Add a Permission for each entity type.
      bean_reset(TRUE);
      foreach (bean_get_types() as $bean_type) {
        $bean_type_name = check_plain($bean_type->type);
        $suggested[] = "view any $bean_type_name bean";
      }
    }
    if (module_exists('better_formats')) {
      $entities = entity_get_info();
      foreach ($entities as $type => $info) {
        if ($info['fieldable']) {
          $suggested[] = 'show format selection for ' . $type;
        }
      }
    }
    if (module_exists('entity_translation')) {
      $workflow = entity_translation_workflow_enabled();
      foreach (entity_get_info() as $entity_type => $info) {
        if ($info['fieldable'] && entity_translation_enabled($entity_type)) {
          $suggested[] = "translate $entity_type entities";

          if ($workflow) {
            // Avoid access control for original values on the current entity.
            if (empty($info['translation']['entity_translation']['skip original values access'])) {
              $suggested[] = "edit $entity_type original values";
            }

            // Avoid access control for shared fields on the current entity.
            if (empty($info['translation']['entity_translation']['skip shared fields access'])) {
              $suggested[] = "edit $entity_type translation shared fields";
            }
          }
        }
      }
    }
    if (module_exists('feeds')) {
      foreach (feeds_importer_load_all() as $importer) {
        $suggested[] = "import $importer->id feeds";
        $suggested[] = "unlock $importer->id feeds";
      }
    }
    if (module_exists('file_entity')) {
      foreach (file_entity_permissions_get_configured_types() as $type) {
        $suggested[] = "edit own $type files";
        $suggested[] = "edit any $type files";
        $suggested[] = "delete own $type files";
        $suggested[] = "delete any $type files";
        $suggested[] = "download own $type files";
        $suggested[] = "download any $type files";
      }
    }
    if (module_exists('flag')) {
      // Reset static cache to ensure all flag permissions are available.
      drupal_static_reset('flag_get_flags');
      $flags = flag_get_flags();
      // Provide flag and unflag permissions for each flag.
      foreach ($flags as $flag_name => $flag) {
        $suggested[] = "flag $flag_name";
        $suggested[] = "unflag $flag_name";
      }
    }
    if (module_exists('node')) {
      foreach (node_permissions_get_configured_types() as $type) {
        $suggested[] = "create $type content";
        $suggested[] = "delete any $type content";
        $suggested[] = "delete own $type content";
        $suggested[] = "edit any $type content";
        $suggested[] = "edit own $type content";
      }
    }
    if (module_exists('registration')) {
      foreach (registration_get_types() as $type_info) {
        $suggested[] = "create $type_info->name registration other anonymous";
        $suggested[] = "create $type_info->name registration other users";
        $suggested[] = "delete any $type_info->name registration";
        $suggested[] = "delete own $type_info->name registration";
        $suggested[] = "edit $type_info->name registration state";
        $suggested[] = "update any $type_info->name registration";
        $suggested[] = "update own $type_info->name registration";
        $suggested[] = "view $type_info->name registration";
        $suggested[] = "view own $type_info->name registration";
      }
    }
    if (module_exists('taxonomy')) {
      foreach (taxonomy_get_vocabularies() as $vocabulary) {
        $suggested[] = 'delete terms in ' . $vocabulary->vid;
        $suggested[] = 'edit terms in ' . $vocabulary->vid;
      }
    }
    if (module_exists('workbench_moderation')) {
      $transitions = workbench_moderation_transitions();

      foreach ($transitions as $transition) {
        $from_state = $transition->from_name;
        $to_state = $transition->to_name;

        // Always set a permission to perform all moderation states.
        $suggested[] = "moderate content from $from_state to $to_state";
      }
    }
  }

  return $suggested;
}
