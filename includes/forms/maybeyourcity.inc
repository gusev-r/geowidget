<?php

/**
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   The form state array.
 *
 * @return
 *   An array containing the title and any custom form elements to be displayed
 *   in the node editing form.
 *
 * @return
 *   The form array.
 */
function geowidget_maybeyourcity_form($form, &$form_state) {
  drupal_add_library('system', 'ui.dialog');
  if (!empty($_SESSION['geowidget'])) {
    $data = $_SESSION['geowidget'] + array('text_maybe_city' => array('#suffix' => ''), 'maybe_you_city' => FALSE);
  } else {
    $city = (array) getCityMaybeCity();
    $data = $city + array('text_maybe_city' => array('#suffix' => '? '), 'maybe_you_city' => TRUE);
  }

  $form['#attributes'] = array('class' => array('geowidget-maybeyourcity-form'));
  $form['#attached']['css'] = array('data' => drupal_get_path('module', 'geowidget') . '/style/css/geowidget.maybeyourcity.css');
  $form['#attached']['js'][] = drupal_get_path('module', 'geowidget') . '/js/maybeyourcity.js';
  $form['#attached']['js'][] = drupal_get_path('module', 'geowidget') . '/js/script.js';
  $form['#prefix'] = '<div id="geo-form-wrapper">';
  $form['#suffix'] = '</div>';
  $form['city_id'] = array(
    '#type' => 'textfield',
    '#default_value' => $data['id'],
  );
  $form['set_city'] = array(
    '#type' => 'submit',
    '#value' => t('set city'),
    '#ajax' => array(
      'callback' => 'geowidget_maybeyourcity_form_set_city_callback',
      'wrapper' => 'geo-form-wrapper',
    ),
    '#attributes' => array('class' => array('btn-set-city')),
  );
  $form['text_maybe_city'] = array(
    '#prefix' => '<div class="wrapper-btn-maybe-city"><div class="prefix-btn-maybe-city">' . t('Ваш город') . '</div> ',
    '#suffix' => '<div class="suffix-btn-maybe-city">' . $data['text_maybe_city']['#suffix'] . '</div></div>',
    '#value' => $data['title'],
    '#type' => 'submit',
    '#ajax' => array('callback' => 'geowidget_maybeyourcity_form_loadcity_callback'),
    '#attributes' => array('class' => array('btn-maybe-city')),
  );
  if ($data['maybe_you_city'] == TRUE) {
    $form['maybe_you_city'] = array(
      '#prefix' => '<div class="wrapper-maybe-you-city">',
      '#suffix' => '</div>',
      '#type' => 'container',
      '#attributes' => array('class' => array('maybe-you-city')),
    );
    $form['maybe_you_city']['text_question'] = array(
      '#markup' => '<div class="text-question">' . t('Ваш город') . ' <br><div class="city">' . $data['title'] . '</div>?</div>',
    );
    $form['maybe_you_city']['yes'] = array(
      '#type' => 'submit',
      '#value' => t('Yes'),
      '#ajax' => array(
        'callback' => 'geowidget_maybeyourcity_form_set_city_callback',
        'wrapper' => 'geo-form-wrapper',
      ),
      '#attributes' => array('class' => array('btn-yes')),
    );
    $form['maybe_you_city']['no'] = array(
      '#type' => 'button',
      '#value' => t('No'),
      '#executes_submit_callback' => FALSE,
      '#attributes' => array('class' => array('btn-no')),
      '#ajax' => array('callback' => 'geowidget_maybeyourcity_form_loadcity_callback', 'wrapper' => 'geo-form-wrapper')
    );
    $form['maybe_you_city']['text_motivation_to_choose'] = array(
      '#markup' => '<div class="text-motivation-to-choose">' . t('От выбранного города зависят цены, наличие товара и способы доставки') . '</div>',
    );
  }
  return $form;
}

/**
 * Addition ajax callback to submit "yes".
 *
 * @see geowidget_maybeyourcity_form()
 */
function geowidget_maybeyourcity_form_set_city_callback($form, $form_state) {
  if ($form['city_id'] != null) {
    $city = city_load($form['city_id']['#value']);
    if ($city) {
      $_SESSION['geowidget'] = array(
        'id' => $city->id,
        'title' => $city->title,
      );
      $form['text_maybe_city']['#value'] = $city->title;
      $form['text_maybe_city']['#suffix'] = '';
      unset($form['maybe_you_city']);
    }
  }
  return $form;
}

/**
 * Addition ajax callback to submit "no" and "text_maybe_city".
 *
 * @see geowidget_maybeyourcity_form()
 */
function geowidget_maybeyourcity_form_loadcity_callback($form, &$form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      array(
        'command' => 'selectCity',
        'arrParam' => _param_geowidget_maybeyourcity_form_loadcity_callback(),
      ),
    ),
  );
}

/**
 *
 * Helper functions for geowidget_maybeyourcity_form_loadcity_callback().
 *
 * @return array
 *   all city.
 * @see geowidget_maybeyourcity_form_loadcity_callback()
 */
function _param_geowidget_maybeyourcity_form_loadcity_callback() {
  $city = &drupal_static(__FUNCTION__);
  if (!isset($city)) {
    if ($cache = cache_get('geowidget')) {
      $city = $cache->data;
    } else {
      $city = db_select('geowidget_city', 'c')
          ->fields('c', array('id', 'parent_id', 'title', 'geoname_id'))
          ->execute()
          ->fetchAll();

      cache_clear_all('geowidget', 'cache', TRUE);
      cache_set('geowidget', $city, 'cache', time() + 360);
    }
  }
  return $city;
}
