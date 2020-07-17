<?php

/**
 * @file
 * Custom functions related to admin pages.
 */

/**
 * Page callback: Nexteuropa User Management settings.
 *
 * @see nexteuropa_user_management_menu()
 */
function _nexteuropa_user_management_settings_form($form, &$form_state) {
  $form['nexteuropa_user_management_um_notification_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification e-mail address for User Management Role users'),
    '#default_value' => variable_get('nexteuropa_user_management_um_notification_email', ''),
    '#description' => t("If you need to have a notification for your User Management Role users, to notify them when a new user registered, you can set here the functional mailbox. If it's empty, no e-mail will be sent. For up-to-date format please check the <a href='https://api.drupal.org/api/drupal/includes%21mail.inc/function/drupal_mail/7.x' target='_blank'>drupal_mail()</a> documentation. Otherwise the following formats are accepted:<br>
user@example.com<br>
user@example.com,anotheruser@example.com<br>
User &lt;user@example.com&gt;<br>
User &lt;user@example.com&gt;,Another User &lt;anotheruser@example.com&gt;<br>
user@example.com,Another User &lt;anotheruser@example.com&gt;"),
  );
  $form['nexteuropa_user_management_um_notification_subject'] = array(
    '#type' => 'textfield',
    '#title' => t("Notification e-mail's subject for User Management Role users"),
    '#default_value' => variable_get('nexteuropa_user_management_um_notification_subject', 'New user registered, waiting for approval.'),
    '#description' => t("Required if notification e-mail address is set. This field supports tokens, if the token module is installed."),
  );
  $body_field_defaults = array(
    'value' => _nexteuropa_user_management_get_default_notification_body(),
    'format' => filter_default_format(),
  );
  $body_field = variable_get('nexteuropa_user_management_um_notification_body', $body_field_defaults);
  $form['nexteuropa_user_management_um_notification_body'] = array(
    '#type' => 'text_format',
    '#title' => t("Notification e-mail's body for User Management Role users"),
    '#default_value' => $body_field['value'],
    '#description' => t('Required if notification e-mail body is set. This field supports tokens, if the token module is installed.'),
    '#format' => $body_field['format'],
  );
  if (module_exists('token')) {
    $form['nexteuropa_user_management_um_notification_body_patterns'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('global_types', 'user'),
      '#dialog' => TRUE,
    );
  }

  return system_settings_form($form);
}

/**
 * Validates the User Management settings form.
 *
 * @param array $form
 *   Contains a renderable form array.
 * @param array $form_state
 *   Contains the form's state.
 */
function _nexteuropa_user_management_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['nexteuropa_user_management_um_notification_email'] !== '') {
    $matches = array();
    // This regex check the e-mail address to be comply with the RFC 5322
    // standard
    // (https://stackoverflow.com/questions/201323/how-to-validate-an-email-address-using-a-regular-expression).
    // Additionally, extended to check also the correct format of if multiple
    // e-mail address was set.
    preg_match('/^(?:(?:(?:[a-z0-9!#$%&\'*+\/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&\'*+\/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])|(?:(?:[^\s,]+[\s]+)+<(?:[a-z0-9!#$%&\'*+\/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&\'*+\/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])>))(?(?=,{1}[^,]{1})(?:,{1})|(?:,{0})))+$/', $form_state['values']['nexteuropa_user_management_um_notification_email'], $matches);

    // Check if e-mail is in the correct format.
    if (count($matches) !== 1) {
      form_error($form['nexteuropa_user_management_um_notification_email'], t("One or more e-mail address or it's format is incorrect! If you want it to be empty, make sure there's no whitespace (i.e. space, tab, etc) in it."));
    }

    if (empty($form_state['values']['nexteuropa_user_management_um_notification_subject'])) {
      form_error($form['nexteuropa_user_management_um_notification_subject'], t('Subject is required if the e-mail address is set.'));
    }

    if (empty($form_state['values']['nexteuropa_user_management_um_notification_body']['value'])) {
      form_error($form['nexteuropa_user_management_um_notification_body']['value'], t('Body is required if the e-mail address is set.'));
    }
  }
}

/**
 * Returns default notification e-mail body.
 *
 * This function is only called until first time the form will be saved.
 * Therefore there's no reason to make a template file from this.
 *
 * @return string
 *   HTML body content.
 */
function _nexteuropa_user_management_get_default_notification_body() {
  return '<p>Dear User managers,</p>
  <p>A new user registered to the site and waiting for your approval.</p>
  <p>Kind regards,<br>
  Webmaster team</p>';
}
